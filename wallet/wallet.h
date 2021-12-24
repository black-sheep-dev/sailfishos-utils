#ifndef WALLET_H
#define WALLET_H

#include <QObject>

class Wallet : public QObject
{
    Q_OBJECT

public:
    explicit Wallet(const QString path, unsigned char *key, unsigned char *iv, QObject *parent = nullptr);

    const QString &path() const;

public slots:
    bool removeEntry(const QString &key);
    QByteArray requestEntry(const QString &key);
    bool reset();
    void setEntry(const QString &key, const QByteArray &value);

private:
    // encryption
    QByteArray encrypt(const QByteArray &data) const;
    QByteArray decrypt(const QByteArray &data) const;

    // functionality
    bool writeEntries(const QHash<QString, QByteArray> &entries) const;
    QHash<QString, QByteArray> getEntries() const;

    unsigned char *m_iv;
    unsigned char *m_key;
    QString m_path;
};

#endif // WALLET_H
