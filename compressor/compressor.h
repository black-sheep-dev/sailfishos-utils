#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <QByteArray>

class Compressor
{
public:
    Compressor() = default;

    static QByteArray gunzip(const QByteArray &data);
};

#endif // COMPRESSOR_H
