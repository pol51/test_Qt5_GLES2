#ifndef __GEOMETRYENGINE_H__
#define __GEOMETRYENGINE_H__

#include <QGLFunctions>
#include <QGLShaderProgram>

class GeometryEngine : protected QGLFunctions
{
  public:
    GeometryEngine() {}
    virtual ~GeometryEngine();

    void init(const qreal ratio);
    void draw(QGLShaderProgram *program);

  protected:
    GLuint vboIds[2];
};

#endif
