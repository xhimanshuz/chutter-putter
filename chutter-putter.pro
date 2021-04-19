CONFIG += c++17
CONFIG -= qt

SOURCES += \
    src/Http_session.cpp \
    src/Ws_session.cpp \
    src/listener.cpp \
    src/main.cpp \
    src/shared_data.cpp

HEADERS += \
    include/Http_session.h \
    include/Ws_session.h \
    include/listener.h \
    include/shared_data.h


INCLUDEPATH += include/
LIBS += -lpthread
