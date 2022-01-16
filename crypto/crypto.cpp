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

    const QByteArray in = compressed ? qUncompress(data) : data;

    QByteArray out = in;

    if (m_longKey.isEmpty()) {
        for (int i = 0; i < in.size(); i++) {
            out[i] = data[i] ^ m_key;
        }

    } else {
        for (int i = 0; i < in.size(); i++) {
            out[i] = in[i] ^ m_longKey[i % (m_longKey.size() / sizeof(char))];
        }
    }

    return out;
}

QByteArray Crypto::encrypt(const QByteArray &data, bool compressed)
{
    if (data.isNull() || data.isEmpty())
        return QByteArray();

    QByteArray out = data;

    if (m_longKey.isEmpty()) {
        for (int i = 0; i < data.size(); i++) {
            out[i] = data[i] ^ m_key;
        }
    } else {
        for (int i = 0; i < data.size(); i++) {
            out[i] = data[i] ^ m_longKey[i % (m_longKey.size() / sizeof(char))];
        }
    }

    if (compressed) {
        out = qCompress(out);
    }

    return out;
}
