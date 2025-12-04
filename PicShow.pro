QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11



# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += D:\Project\QtPrj\Mitsubishi\PicShow\view
              D:\Project\QtPrj\Mitsubishi\PicShow\mcprotocol

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    connection.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    mcprotocol/melsec_helper.c \
    mcprotocol/melsec_mc_bin.c \
    mcprotocol/melsec_mc_comm.c \
    mcprotocol/socket.c \
    mcprotocol/utill.c \
    parammeterchange.cpp \
    scancodedialog.cpp \
    senceview.cpp \
    view/diagramitem.cpp \
    view/diagramscene.cpp \
    view/diagramtextitem.cpp \
    view/diagramview.cpp \
    view/undosystem.cpp


HEADERS += \
    connection.h \
    logindialog.h \
    mainwindow.h \
    mcprotocol/melsec_helper.h \
    mcprotocol/melsec_mc_bin.h \
    mcprotocol/melsec_mc_bin_private.h \
    mcprotocol/melsec_mc_comm.h \
    mcprotocol/socket.h \
    mcprotocol/typedef.h \
    mcprotocol/utill.h \
    scancodedialog.h \
    view/diagramitem.h \
    view/diagramscene.h \
    view/diagramtextitem.h \
    view/diagramview.h \
    view/undosystem.h


FORMS += \
    logindialog.ui \
    mainwindow.ui \
    scancode.ui

LIBS += -Ldll -lws2_32
LIBS+= -luser32           # 使用WindowsAPI需要链接库

RC_ICONS = icon.ico
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
