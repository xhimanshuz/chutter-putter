CONFIG += c++17
CONFIG -= qt

SOURCES += \
    src/session.cpp \
    src/Http_session.cpp \
    src/listener.cpp \
    src/main.cpp \
    src/shared_data.cpp

HEADERS += \
    include/Http_session.h \
    include/listener.h \
    include/shared_data.h \
    include/session.h


INCLUDEPATH += include/
LIBS += -lpthread -lboost_json

DISTFILES += doc/protocol.json

TARGET += $$PWD/build/
