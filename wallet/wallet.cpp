#include "wallet.h"

#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QHash>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

constexpr quint64 SFOS_UTILS_WALLET_MAGIC = 0x53464f535557; // SFOSUW
constexpr quint16 SFOS_UTILS_WALLET_VERSION = 1;

Wallet::Wallet(const QString path, unsigned char *key, unsigned char *iv, QObject *parent) :
    QObject(parent),
    m_iv(iv),
    m_key(key),
    m_path(path)
{

}

const QString &Wallet::path() const
{
    return m_path;
}

bool Wallet::removeEntry(const QString &key)
{
    auto entries = getEntries();
    entries.remove(key);

    return writeEntries(entries);
}

QByteArray Wallet::requestEntry(const QString &key)
{
    const auto entries = getEntries();

    if (!entries.keys().contains(key)) {
        qWarning() << "Entry is not available";
        return QByteArray();
    }

    return decrypt(entries.value(key));
}

bool Wallet::reset()
{
    return QFile(m_path).remove();
}

void Wallet::setEntry(const QString &key, const QByteArray &value)
{
    QHash<QString, QByteArray> entries = getEntries();

    entries.insert(key, encrypt(value));

    writeEntries(entries);
}

QByteArray Wallet::encrypt(const QByteArray &data) const
{
    unsigned char ciphertext[data.length() * 2];

    EVP_CIPHER_CTX *ctx{nullptr};

    int len{0};

    if (!(ctx = EVP_CIPHER_CTX_new())) {
        qWarning() << "Could not create cipher context";
        return QByteArray();
    }

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, m_key, m_iv)) {
        qWarning() << "Could not init encryption";
        return QByteArray();
    }

    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, (unsigned char *)(data.data()), data.length())) {
        qWarning() << "Encryption failed";
        return QByteArray();
    }
    int ciphertextLen = len;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) {
        qWarning() << "Finalize encryption failed";
        return QByteArray();
    }
    ciphertextLen += len;

    EVP_CIPHER_CTX_free(ctx);

    QByteArray out((char *)ciphertext);
    out.resize(ciphertextLen);

    return out;
}

QByteArray Wallet::decrypt(const QByteArray &data) const
{
    unsigned char plain[data.length()];

    EVP_CIPHER_CTX *ctx{nullptr};

    int len{0};

    if (!(ctx = EVP_CIPHER_CTX_new())) {
        qWarning() << "Could not create cipher context";
        return QByteArray();
    }

    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, m_key, m_iv)) {
        qWarning() << "Could not init decryption";
        return QByteArray();
    }

    if (1 != EVP_DecryptUpdate(ctx, plain, &len, (unsigned char *)(data.data()), data.length())) {
        qWarning() << "Decryption failed";
        return QByteArray();
    }
    int plainLen = len;

    if (1 != EVP_DecryptFinal_ex(ctx, plain + len, &len)) {
        qWarning() << "Finalize decryption failed";
        return QByteArray();
    }
    plainLen += len;

    QByteArray out((char *)plain);
    out.resize(plainLen);

    return out;
}

bool Wallet::writeEntries(const QHash<QString, QByteArray> &entries) const
{
    QFile wallet(m_path);

    if (!wallet.open(QIODevice::WriteOnly)) {
        qCritical() << "Could not open wallet: " << m_path;
        return false;
    }

    QDataStream stream(&wallet);
    stream.setVersion(QDataStream::Qt_5_6);

    stream << SFOS_UTILS_WALLET_MAGIC;
    stream << SFOS_UTILS_WALLET_VERSION;
    stream << entries;

    wallet.close();

    return true;
}

QHash<QString, QByteArray> Wallet::getEntries() const
{
    QHash<QString, QByteArray> entries;

    QFile wallet(m_path);

    if (!wallet.open(QIODevice::ReadOnly)) {
        qCritical() << "Could not open wallet: " << m_path;
        return entries;
    }

    QDataStream stream(&wallet);
    stream.setVersion(QDataStream::Qt_5_6);

    // read file magic number
    quint64 magic{0};
    stream >> magic;

    if (magic != SFOS_UTILS_WALLET_MAGIC) {
        wallet.close();
        qWarning() << "File is no wallet: " << m_path;
        return entries;
    }

    // read file version
    quint16 version{0};
    stream >> version;

    // read entries;
    stream >> entries;

    // close and return;
    wallet.close();
    return entries;
}
