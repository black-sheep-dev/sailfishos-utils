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

Crypto::Crypto(const QByteArray &key) :
    m_longKey(key)
{

}

QByteArray Crypto::decrypt(const QByteArray &data, bool compressed)
{
    if (data.isNull() || data.isEmpty())
        return QByteArray();

    QByteArray in = compressed ? qUncompress(data) : data;

    QByteArray out;

    if (m_longKey.isEmpty()) {
        int key = m_key;

        for (const char c : in) {
            int a = key ^ static_cast<int>(c);
            key = static_cast<int>(c);
            out.append(static_cast<char>(a));
        }

    } else {
        for (const char k : m_longKey) {
            int key = static_cast<int>(k);

            for (const char c : in) {
                int a = key ^ static_cast<int>(c);
                key = static_cast<int>(c);
                out.append(static_cast<char>(a));
            }

            in = out;
            out.clear();
        }
    }

    return out;
}

QByteArray Crypto::encrypt(const QByteArray &data, bool compressed)
{
    if (data.isNull() || data.isEmpty())
        return QByteArray();

    QByteArray out;

    int key = m_key;

    if (m_longKey.isEmpty()) {
        for (const char c : data) {
            int a = key ^ static_cast<int>(c);
            key = a;
            out.append(static_cast<char>(a));
        }
    } else {
        for (const char k : m_longKey) {
            int key = static_cast<int>(k);

            QByteArray in = data;

            for (const char c : in) {
                int a = key ^ static_cast<int>(c);
                key = static_cast<int>(c);
                out.append(static_cast<char>(a));
            }

            in = out;
            out.clear();
        }
    }

    if (compressed) {
        out = qCompress(out);
    }

    return out;
}
