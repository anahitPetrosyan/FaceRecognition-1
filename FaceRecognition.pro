QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += C:/Users/Gevorg/Desktop/Gevorg/programming/classic/dlib-19.18/source
LIBS += -LC:\\Users\\Gevorg\\Desktop\\Gevorg\\programming\\classic\\dlib-19.18\\build\\dlib\\Release\\
    -ldlib19.18.0_release_64bit_msvc1916

INCLUDEPATH += C:/Users/Gevorg/Desktop/Gevorg/programming/classic/opencv/build/include
LIBS += -LC:\\Users\\Gevorg\\Desktop\\Gevorg\\programming\\classic\\opencv\\build\\x64\\vc15\\lib\
    -lopencv_world412
INCLUDEPATH += C:\\Users\\Gevorg\\Desktop\\Gevorg\\programming\\classic\\sqlite
LIBS += -LC:\\Users\\Gevorg\\Desktop\\Gevorg\\programming\\classic\\sqlite -lsqlite3

SOURCES += \
    ../../Desktop/Gevorg/programming/classic/dlib-19.18/source/dlib/all/source.cpp \
    ../../Desktop/Gevorg/programming/classic/sqlite/sqlite3.c \
    UpdateInfo.cpp \
    database.cpp \
    faceRecognition.cpp \
    main.cpp

HEADERS += \
    UpdateInfo.h \
    database.h \
    faceRecognition.h \
    takeimage.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
