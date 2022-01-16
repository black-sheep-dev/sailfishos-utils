#ifndef CRYPTO_H
#define CRYPTO_H

#include <QByteArray>

class Crypto
{
public:
    Crypto(quint8 key);
    Crypto(const QByteArray &key);

    QByteArray decrypt(const QByteArray &data, bool compressed = false);
    QByteArray encrypt(const QByteArray &data, bool compressed = false);

private:
    quint8 m_key{111};
    QByteArray m_longKey;
};

#endif // CRYPTO_H
