TEMPLATE    = app
QT         += opengl multimedia

INCLUDEPATH +=  /usr/include/glm
INCLUDEPATH += ./Model

FORMS += MyForm.ui

HEADERS += MyForm.h LL4GLWidget.h MyGLWidget.h GameObject.h Car.h

SOURCES += main.cpp  MyForm.cpp \
        LL4GLWidget.cpp MyGLWidget.cpp GameObject.cpp Car.cpp ./Model/model.cpp


