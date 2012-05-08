#-------------------------------------------------
#
# Project created by QtCreator 2011-07-01T00:00:45
#
#-------------------------------------------------

QT       += core gui

TARGET = ClangIDE
TEMPLATE = app


SOURCES += main.cpp\
        clangide.cpp astprinter.cpp ppcontext.cpp \
    clangparser.cpp qdiagnosticclient.cpp

HEADERS  += clangide.h \
    clangparser.h qdiagnosticclient.h

FORMS    += clangide.ui

QMAKE_CXXFLAGS += -fno-rtti -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS
LIBS += -lclangRewrite -lclangParse -lclangSema -lclangAnalysis -lclangAST -lclangFrontend -lclangLex -lclangBasic `llvm-config --libs`
