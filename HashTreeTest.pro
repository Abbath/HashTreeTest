TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

QMAKE_CXXFLAGS += -std=c++11
LIBS += -pthread

HEADERS += \
    propersubsets.hpp \
    hashtree_sup.hpp

OTHER_FILES +=
