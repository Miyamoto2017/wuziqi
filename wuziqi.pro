QT += widgets

CONFIG += c++17

TARGET = wuziqi
TEMPLATE = app

SOURCES += \
    main.cpp \
    src/MainWindow.cpp \
    src/GameBoard.cpp \
    src/GameLogic.cpp \
    src/AIPlayer.cpp

HEADERS += \
    src/MainWindow.h \
    src/GameBoard.h \
    src/GameLogic.h \
    src/AIPlayer.h

RESOURCES += resources.qrc
