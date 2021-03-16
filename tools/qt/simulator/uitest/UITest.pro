
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DEFINES += QT_COMPILER

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = ../

FORMS += \
    main_widget.ui

LIBS += -L$$DESTDIR/libs \
        -lharfbuzz \
        -licu \

TRANSLATIONS += helloqt_zh_CN.ts

# Default rules for deployment.
qnx: target.path =/tmp/$${TARGET}/bin
else: unix:!android: target.path =/opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SOURCES += \
    ../drivers/display/gui_thread.cpp \
    ../drivers/display/monitor.cpp \
    ../drivers/display/task_thread.cpp \
    ../drivers/indev/mouse_input.cpp \
    ../drivers/indev/mousewheel_input.cpp \
    main.cpp \
    main_widget.cpp

HEADERS += \
    ../drivers/config.h \
    ../drivers/display/gui_thread.h \
    ../drivers/display/task_thread.h \
    ../drivers/display/monitor.h \
    ../drivers/indev/mouse_input.h \
    ../drivers/indev/mousewheel_input.h \
    main_widget.h

INCLUDEPATH += \
    ../../../../frameworks \
    ../../../../../utils/interfaces/innerkits \
    ../../../../../utils/interfaces/kits \
    ../../../../../ui/interfaces/innerkits \
    ../../../../../ui/interfaces/kits \
    ../../../../../../../third_party/freetype/include \
    ../drivers/display \
    ../drivers/indev \
    ../drivers

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../test/release/ -ltest
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../test/debug/ -ltest

INCLUDEPATH += $$PWD/../test
DEPENDPATH += $$PWD/../test

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../test/release/libtest.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../test/debug/libtest.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../test/release/test.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../test/debug/test.lib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libui/release/ -llibui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libui/debug/ -llibui

INCLUDEPATH += $$PWD/../libui
DEPENDPATH += $$PWD/../libui

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libui/release/liblibui.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libui/debug/liblibui.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libui/release/libui.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libui/debug/libui.lib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../third_party/freetype/release/ -lfreetype
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../third_party/freetype/debug/ -lfreetype

INCLUDEPATH += $$PWD/../third_party/freetype
DEPENDPATH += $$PWD/../third_party/freetype

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/freetype/release/libfreetype.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/freetype/debug/libfreetype.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/freetype/release/freetype.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/freetype/debug/freetype.lib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../third_party/libjpeg/release/ -llibjpeg
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../third_party/libjpeg/debug/ -llibjpeg

INCLUDEPATH += $$PWD/../third_party/libjpeg
DEPENDPATH += $$PWD/../third_party/libjpeg

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/libjpeg/release/liblibjpeg.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/libjpeg/debug/liblibjpeg.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/libjpeg/release/libjpeg.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/libjpeg/debug/libjpeg.lib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../third_party/libpng/release/ -llibpng
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../third_party/libpng/debug/ -llibpng

INCLUDEPATH += $$PWD/../third_party/libpng
DEPENDPATH += $$PWD/../third_party/libpng

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/libpng/release/liblibpng.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/libpng/debug/liblibpng.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/libpng/release/libpng.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/libpng/debug/libpng.lib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../third_party/qrcodegen/release/ -lqrcodegen
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../third_party/qrcodegen/debug/ -lqrcodegen

INCLUDEPATH += $$PWD/../third_party/qrcodegen
DEPENDPATH += $$PWD/../third_party/qrcodegen

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/qrcodegen/release/libqrcodegen.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/qrcodegen/debug/libqrcodegen.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/qrcodegen/release/qrcodegen.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/qrcodegen/debug/qrcodegen.lib

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../third_party/zlib/release/ -lzlib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../third_party/zlib/debug/ -lzlib

INCLUDEPATH += $$PWD/../third_party/zlib
DEPENDPATH += $$PWD/../third_party/zlib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/zlib/release/libzlib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/zlib/debug/libzlib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/zlib/release/zlib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../third_party/zlib/debug/zlib.lib
