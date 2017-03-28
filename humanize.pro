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
