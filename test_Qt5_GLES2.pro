QT += core gui widgets opengl

TARGET = test_Qt5_GLES2
TEMPLATE = app

SOURCES += main.cpp

SOURCES += \
  mainwidget.cpp \
  geometryengine.cpp

HEADERS += \
  mainwidget.h \
  geometryengine.h

RESOURCES += \
  shaders.qrc

OTHER_FILES += \
  fshader.glsl \
  vshader.glsl
