#include "crypto.h"

#include <QByteArray>
#include <QtDebug>
#include <QtGlobal>
#include <QDateTime>
#include <QCryptographicHash>
#include <QDataStream>

Crypto::Crypto(quint8 key) :
    m_key(key)
{

}

QByteArray Crypto::decrypt(const QByteArray &data, bool compressed)
{
    if (data.isNull() || data.isEmpty())
        return QByteArray();

    const QByteArray in = compressed ? qUncompress(data) : data;

    QByteArray out;

    int key = m_key;

    for (const char c : in) {
        int a = key ^ static_cast<int>(c);
        key = static_cast<int>(c);
        out.append(static_cast<char>(a));
    }

    return out;
}

QByteArray Crypto::encrypt(const QByteArray &data, bool compressed)
{
    if (data.isNull() || data.isEmpty())
        return QByteArray();

    QByteArray out;

    int key = m_key;

    for (const char c : data) {
        int a = key ^ static_cast<int>(c);
        key = a;
        out.append(static_cast<char>(a));
    }

    if (compressed) {
        out = qCompress(out);
    }

    return out;
}
