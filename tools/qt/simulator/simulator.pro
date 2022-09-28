TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    third_party/giflib \
    third_party/harfbuzz \
    third_party/icu \
    third_party/libjpeg-turbo \
    third_party/zlib \
    third_party/libpng \
    third_party/freetype \
    third_party/qrcodegen \
    libui \
    test \
    autotest/auto_test.pro \
    uitest/UITest.pro

HEADERS += \
    ../../../test/framework/common/ui_test_canvas_common.h
