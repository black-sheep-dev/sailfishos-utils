CONFIG += link_pkgconfig
PKGCONFIG += openssl 

INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

HEADERS += \
    $$PWD/wallet.h

SOURCES += \
    $$PWD/wallet.cpp  
