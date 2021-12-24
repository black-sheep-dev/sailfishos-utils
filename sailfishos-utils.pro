TEMPLATE = lib
CONFIG += static
CONFIG -= console c++11
CONFIG -= app_bundle

TARGET = sailfishos-utils

QT += core

include(compressor/compressor.pri)
include(crypto/crypto.pri)
include(wallet/wallet.pri)

DISTFILES += \
    rpm/sailfishos-utils.spec
