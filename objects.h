#ifndef _OBJECTS_H
#define _OBJECTS_H

#include <QGLWidget>
#include <QObject>

#if defined(__APPLE__) || defined(MACOSX)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Objects : public QObject {
public:
  Objects(QGLWidget *parent = 0);
  // routines for drawing geometric primitives :  platonic solids
  void drawTetrahedron();
  void drawCube();
  void drawOctahedron();
  void solidCone(GLdouble base, GLdouble height, GLint slices, GLint stacks);
  void drawDodecahedron();
  void drawIcosahedron();
  void drawAxes();
  void drawGrid(GLfloat *colour, int nGridlines);
  void drawSphere(float radius, int nSegments, int nSlices);
  void drawCylinder(float radius, float height, int nSegments);

private:
  QGLWidget *qGLWidget;
};
#endif // _OBJECTS_H
