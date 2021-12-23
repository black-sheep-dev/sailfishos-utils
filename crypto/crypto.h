/*
Copyright (c) 2011, Andre Somers
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Rathenau Instituut, Andre Somers nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANDRE SOMERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR #######; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef CRYPTO_H
#define CRYPTO_H

#include <QString>
#include <QVector>
#include <QFlags>

class Crypto
{
public:
    enum CompressionMode {
        CompressionAuto,
        CompressionAlways,
        CompressionNever
    };

    enum IntegrityProtectionMode {
        ProtectionNone,
        ProtectionChecksum,
        ProtectionHash
    };

    enum Error {
        ErrorNoError,
        ErrorNoKeySet,
        ErrorUnknownVersion,
        ErrorIntegrityFailed
    };

    enum CryptoFlag{
        CryptoFlagNone          = 0x00,
        CryptoFlagCompression   = 0x01,
        CryptoFlagChecksum      = 0x02,
        CryptoFlagHash          = 0x04
    };
    Q_DECLARE_FLAGS(CryptoFlags, CryptoFlag);

    Crypto();
    explicit Crypto(quint64 key);

    void setKey(quint64 key);
    bool hasKey() const;
    void setCompressionMode(CompressionMode mode);
    CompressionMode compressionMode() const;
    void setIntegrityPRotectionMode(IntegrityProtectionMode mode);
    IntegrityProtectionMode integrityProtectionMode() const;
    Error lastError() const;
    QString encryptToString(const QString &plaintext);
    QString encryptToString(QByteArray plaintext);
    QByteArray encryptToByteArray(const QString &plaintext);
    QByteArray encryptToByteArray(QByteArray plaintext);
    QString decryptToString(const QString &cyphertext);
    QString decryptToString(QByteArray cyphertext);
    QByteArray decryptToByteArray(const QString &cyphertext);
    QByteArray decryptToByteArray(QByteArray cyphertext);

private:
    void splitKey();

    quint64 m_key{0};
    QVector<char> m_keyParts;
    CompressionMode m_compressionMode{CompressionMode::CompressionAuto};
    IntegrityProtectionMode m_protectionMode{IntegrityProtectionMode::ProtectionChecksum};
    Error m_lastError{Error::ErrorNoError};
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Crypto::CryptoFlags)

#endif // CRYPTO_H
