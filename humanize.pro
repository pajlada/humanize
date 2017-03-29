TEMPLATE = app
CONFIG += c++14
CONFIG -= app_bundle

INCLUDEPATH += ./include

SOURCES += \
    src/main.cpp

HEADERS += \
    include/humanize/time.hpp \
    include/humanize/numbers.hpp \
    include/humanize/format_string.hpp

win32-msvc* {
    QMAKE_CXXFLAGS_WARN_ON = /W4 /wd4996
    QMAKE_CXXFLAGS_WARN_ON += /wd4996 # Ignore "unsafe" warnings
}
