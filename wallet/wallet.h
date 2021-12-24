#ifndef WALLET_H
#define WALLET_H

#include <QObject>

class Wallet : public QObject
{
    Q_OBJECT

public:
    explicit Wallet(const QString path, unsigned char *key, unsigned char *iv, QObject *parent = nullptr);

    const QString &path() const;

signals:
    void entryAvailable(const QString &key, const QByteArray &value);

public slots:
    bool requestEntry(const QString &key);
    void setEntry(const QString &key, const QByteArray &value);

private:
    QByteArray encrypt(const QByteArray &data) const;
    QByteArray decrypt(const QByteArray &data) const;

    QHash<QString, QByteArray> getEntries() const;

    unsigned char *m_iv;
    unsigned char *m_key;
    QString m_path;
};

#endif // WALLET_H
