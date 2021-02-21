QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += -w
CONFIG += -o
CONFIG += -g

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#CONFIG(release, debug|release) {
#    DEFINES+=-DRELEASE
#    message(Release)
#}

CONFIG(debug, debug|release) {
    DEFINES+=__DEBUG__
    message(Debug)
}
SOURCES += \
    MakeMockFile.cpp \
    main.cpp \
    gmockgen.cpp \
    worker.cpp

HEADERS += \
    MakeMockFile.h \
    gmockgen.h \
    worker.h

FORMS += \
    gmockgen.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    icon.ico
RC_ICONS = icon.ico
