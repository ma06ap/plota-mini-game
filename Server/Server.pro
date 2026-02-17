QT       += core network widgets
QT       -= gui

TARGET = PlotaServer
CONFIG += c++17
TEMPLATE = app

INCLUDEPATH += $$PWD/..

SOURCES += \
    main.cpp \
    GameServer.cpp \
    ServerWindow.cpp \
    $$PWD/../Backend/Games/Board.cpp \
    $$PWD/../Backend/Games/Game.cpp \
    $$PWD/../Backend/Games/Piece.cpp \
    $$PWD/../Backend/Games/Turn.cpp \
    $$PWD/../Backend/Games/Status.cpp \
    $$PWD/../Backend/Games/Othello/Othello.cpp \
    $$PWD/../Backend/Games/Checkers/Checkers.cpp \
    $$PWD/../Backend/Games/Connect-4/Connect-4.cpp \
    $$PWD/../Backend/Tools/Time.cpp

HEADERS += \
    GameServer.h \
    ServerWindow.h \
    $$PWD/../Backend/Games/Board.h \
    $$PWD/../Backend/Games/Game.h \
    $$PWD/../Backend/Games/Piece.h \
    $$PWD/../Backend/Games/Turn.h \
    $$PWD/../Backend/Games/Status.h \
    $$PWD/../Backend/Games/Location.h \
    $$PWD/../Backend/Games/Othello/Othello.h \
    $$PWD/../Backend/Games/Checkers/Checkers.h \
    $$PWD/../Backend/Games/Connect-4/Connect-4.h \
    $$PWD/../Backend/Tools/Time.h

FORMS += \
    ServerWindow.ui
