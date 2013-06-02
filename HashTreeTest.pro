TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    1.cpp

QMAKE_CXXFLAGS += -std=c++11
LIBS += -pthread

HEADERS += \
    hashtree.hpp \
    propersubsets.hpp \
    hashtree_sup.hpp
