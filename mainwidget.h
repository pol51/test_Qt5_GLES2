#ifndef __MAINWIDGET_H__
#define __MAINWIDGET_H__

#include "geometryengine.h"

#include <QGLWidget>
#include <QGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QGLShaderProgram>

class GeometryEngine;

class MainWidget : public QGLWidget, protected QGLFunctions
{
  Q_OBJECT

  public:
    explicit MainWidget(QString image, QWidget *parent = NULL);
    ~MainWidget();

  protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void initShaders();
    void initTextures();

  protected:
    bool moving = false;
    qreal x = 0.f;
    qreal y = 0.f;

    qreal imageRatio = 1.f;

    qreal xRatio = 1.f;
    qreal yRatio = 1.f;

    QString imageFilename;

    QGLShaderProgram program;
    GeometryEngine geometries;

    GLuint texture;

    QMatrix4x4 projection;

    QVector2D mousePressPosition;
    qreal distance = 2.5f;
};

#endif
