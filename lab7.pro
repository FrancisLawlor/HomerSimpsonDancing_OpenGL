QT       += core gui opengl

TARGET = Lab7
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    glwidget.cpp \
    objects.cpp \
    PPMImage.cpp \
    RGBValue.cpp \
    quat.cpp \
    arcBall.cpp \
    vector.cpp \
    point.cpp


HEADERS  += mainwindow.h \
    glwidget.h \
    objects.h \
    PPMImage.h \
    RGBValue.h \
    quat.h \
    arcBall.h \
    vector.h \
    point.h

FORMS    += mainwindow.ui

mac: LIBS += -framework GLUT
unix:!macx:!symbian: LIBS += -lGL -lGLU -lglut

RESOURCES += \
    resources.qrc
