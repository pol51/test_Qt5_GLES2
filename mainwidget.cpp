#include "mainwidget.h"

#include <QMouseEvent>

#include <math.h>
#include <locale.h>

MainWidget::MainWidget(QString image, QWidget *parent) :
  QGLWidget(parent), imageFilename(image)
{
}

MainWidget::~MainWidget()
{
  deleteTexture(texture);
}

void MainWidget::mousePressEvent(QMouseEvent *e)
{
  moving = true;
  mousePressPosition = QVector2D(e->localPos());
}

void MainWidget::mouseMoveEvent(QMouseEvent *e)
{
  QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;
  x += diff.x() * xRatio;
  y -= diff.y() * yRatio;

  mousePressPosition = QVector2D(e->localPos());

  updateGL();
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
  moving = false;
  mousePressPosition = QVector2D(e->localPos());
}

void MainWidget::wheelEvent(QWheelEvent *e)
{
  if (e->orientation() == Qt::Vertical)
  {
    distance += e->delta() / 1200.f * distance; // near -> slow / far -> fast
    if (distance < .01f) distance = .01f;
    if (distance > 3.f)  distance = 3.f;

    const qreal W(width()), H(height() ? height() : 1);
    const qreal Aspect = W / H;

    xRatio = (distance / ((projection.column(0).x()) / 2.))/W;
    yRatio = (distance / ((projection.column(0).x()) / 2.))/H / Aspect;

    updateGL();
  }
}

void MainWidget::initializeGL()
{
  initializeGLFunctions();
  qglClearColor(Qt::black);
  initShaders();
  initTextures();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  geometries.init(imageRatio);
}

void MainWidget::initShaders()
{
  setlocale(LC_NUMERIC, "C");

  if (!program.addShaderFromSourceFile(QGLShader::Vertex,   ":/vshader.glsl"))  close();
  if (!program.addShaderFromSourceFile(QGLShader::Fragment, ":/fshader.glsl"))  close();

  if (!program.link()) close();
  if (!program.bind()) close();

  setlocale(LC_ALL, "");
}

void MainWidget::initTextures()
{
  glEnable(GL_TEXTURE_2D);
  QImage Image(imageFilename);
  texture = bindTexture(Image);

  imageRatio = qreal(Image.width()) / (Image.height() ? Image.height() : 1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void MainWidget::resizeGL(int w, int h)
{
  glViewport(0, 0, w, h);

  const qreal Aspect = qreal(w) / qreal(h ? h : 1);
  const qreal ZNear = 0.0001, ZFar = 3.5, Fov = 45;

  projection.setToIdentity();
  projection.perspective(Fov, Aspect, ZNear, ZFar);

  xRatio = (distance / ((projection.column(0).x()) / 2.))/w;
  yRatio = (distance / ((projection.column(0).x()) / 2.))/h / Aspect;
}

void MainWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  QMatrix4x4 Translation;
  Translation.translate(x, y, -distance);

  program.setUniformValue("mvp_matrix", projection * Translation);
  program.setUniformValue("texture", 0);

  geometries.draw(&program);
}
