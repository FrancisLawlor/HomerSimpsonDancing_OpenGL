#include "objects.h"
#include "point.h"
#include <cmath>

const double pi = 3.14159265358979f;

static GLfloat black[] = {0.0, 0.0, 0.0, 1.0};
static GLfloat grey[] = {0.5, 0.5, 0.5, 1.0};

Objects::Objects(QGLWidget *parent) : QObject(parent) { qGLWidget = parent; }

/*
 * Implement all the subsequent functions routines for drawing geometric
 * primitives
 * platonic solids
 */

void Objects::drawTetrahedron() {
  Point vertices[4] = {Point(-1.0, -1.0, -1.0, 0.0), Point(-1.0, 1.0, 1.0, 0.0),
                       Point(1.0, -1.0, 1.0, 0.0), Point(1.0, 1.0, -1.0, 0.0)};

  int faces[4][3] = {{0, 2, 1}, {1, 3, 0}, {2, 3, 1}, {3, 2, 0}};

  glBegin(GL_TRIANGLES);
  for (int face = 0; face < 4; face++) { // per face
    Vector v = vertices[faces[face][1]] - vertices[faces[face][0]];
    Vector w = vertices[faces[face][2]] - vertices[faces[face][0]];
    Vector normalised = v.cross(w).normalised();
    glNormal3f(normalised.dx, normalised.dy, normalised.dz);
    glVertex3f(vertices[faces[face][0]].x, vertices[faces[face][0]].y,
               vertices[faces[face][0]].z);
    glVertex3f(vertices[faces[face][1]].x, vertices[faces[face][1]].y,
               vertices[faces[face][1]].z);
    glVertex3f(vertices[faces[face][2]].x, vertices[faces[face][2]].y,
               vertices[faces[face][2]].z);
  } // per face
  glEnd();
}

void Objects::drawCube() {

  Point vertices[8] = {
      Point(-1.0, -1.0, -1.0, 0.0), Point(-1.0, -1.0, 1.0, 0.0),
      Point(-1.0, 1.0, -1.0, 0.0),  Point(-1.0, 1.0, 1.0, 0.0),
      Point(1.0, -1.0, -1.0, 0.0),  Point(1.0, -1.0, 1.0, 0.0),
      Point(1.0, 1.0, -1.0, 0.0),   Point(1.0, 1.0, 1.0, 0.0)};

  int faces[6][4] = {{0, 4, 5, 1}, {0, 2, 6, 4}, {0, 1, 3, 2},
                     {4, 6, 7, 5}, {1, 5, 7, 3}, {2, 3, 7, 6}};

  glBegin(GL_QUADS);
  for (int face = 0; face < 6; face++) { // per face
    Vector v = vertices[faces[face][1]] - vertices[faces[face][0]];
    Vector w = vertices[faces[face][3]] - vertices[faces[face][0]];
    Vector normalised = v.cross(w).normalised();
    glNormal3f(normalised.dx, normalised.dy, normalised.dz);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(vertices[faces[face][0]].x, vertices[faces[face][0]].y,
               vertices[faces[face][0]].z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(vertices[faces[face][1]].x, vertices[faces[face][1]].y,
               vertices[faces[face][1]].z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(vertices[faces[face][2]].x, vertices[faces[face][2]].y,
               vertices[faces[face][2]].z);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(vertices[faces[face][3]].x, vertices[faces[face][3]].y,
               vertices[faces[face][3]].z);
  } // per face
  glEnd();
}

void Objects::drawOctahedron() {

  Point vertices[6] = {Point(-1.0, 0.0, 0.0, 0.0), Point(0.0, -1.0, 0.0, 0.0),
                       Point(0.0, 0.0, -1.0, 0.0), Point(1.0, 0.0, 0.0, 0.0),
                       Point(0.0, 1.0, 0.0, 0.0),  Point(0.0, 0.0, 1.0, 0.0)};

  int faces[8][3] = {{3, 4, 5}, {4, 0, 5}, {0, 1, 5}, {1, 4, 5},
                     {0, 4, 2}, {4, 3, 2}, {3, 1, 2}, {1, 0, 2}};

  glBegin(GL_TRIANGLES);
  for (int face = 0; face < 8; face++) { // per face
    Vector v = vertices[faces[face][1]] - vertices[faces[face][0]];
    Vector w = vertices[faces[face][2]] - vertices[faces[face][0]];
    Vector normalised = v.cross(w).normalised();
    glNormal3f(normalised.dx, normalised.dy, normalised.dz);
    glVertex3f(vertices[faces[face][0]].x, vertices[faces[face][0]].y,
               vertices[faces[face][0]].z);
    glVertex3f(vertices[faces[face][1]].x, vertices[faces[face][1]].y,
               vertices[faces[face][1]].z);
    glVertex3f(vertices[faces[face][2]].x, vertices[faces[face][2]].y,
               vertices[faces[face][2]].z);
  } // per face
  glEnd();
}

void Objects::solidCone(GLdouble base, GLdouble height, GLint slices,
                        GLint stacks) {
  glBegin(GL_LINE_LOOP);
  GLUquadricObj *quadric = gluNewQuadric();
  gluQuadricDrawStyle(quadric, GLU_FILL);
  gluCylinder(quadric, base, 0, height, slices, stacks);

  gluDeleteQuadric(quadric);
  glEnd();
}

void Objects::drawDodecahedron() {
  // glutSolidDodecahedron();
}

void Objects::drawIcosahedron() {
  // glutSolidIcosahedron();
}

void Objects::drawCylinder(float radius, float height, int nSegments) {

  glBegin(GL_TRIANGLES);
  for (float i = 0.0; i <= nSegments; i += 1.0f) { /* theta loop */
    float theta = (2.0f * pi * i) / (float)nSegments;
    float nextTheta = (2.0f * pi * (i + 1.0)) / (float)nSegments;

    float x1 = cos(theta) * radius, y1 = sin(theta) * radius;
    float x2 = cos(nextTheta) * radius, y2 = sin(nextTheta) * radius;

    // first triangle of the panel
    glNormal3f(x1, y1, 0.0f);
    glVertex3f(x1, y1, 0.0f);
    glNormal3f(x2, y2, 1.0f);
    glVertex3f(x2, y2, height);
    glNormal3f(x1, y1, 1.0f);
    glVertex3f(x1, y1, height);

    // second triangle of the panel
    glNormal3f(x1, y1, 0.0f);
    glVertex3f(x1, y1, 0.0f);
    glNormal3f(x2, y2, 0.0f);
    glVertex3f(x2, y2, 0.0f);
    glNormal3f(x2, y2, 1.0f);
    glVertex3f(x2, y2, height);

    // for a closed cylinder bottom triangle
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(x1, y1, 0.0f);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(x2, y2, 0.0f);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.0f, 0.0f, 0.0f);

    // top triangle
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x1, y1, height);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x2, y2, height);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, height);
  } /* theta loop */

  glEnd();
}

void Objects::drawSphere(float radius, int nSegments, int nSlices) {
  float x, y, z;
  float s, t; // texture coordinates

  float inctheta = (2.0f * pi) / float(nSlices);
  float incphi = pi / float(nSegments);

  glBegin(GL_QUADS);
  for (float theta = -pi; theta < pi; theta += inctheta) {
    for (float phi = -(pi / 2.0f); phi < (pi / 2.0f); phi += incphi) {
      x = cos(phi) * cos(theta) * radius;
      y = cos(phi) * sin(theta) * radius;
      z = sin(phi) * radius;
      t = (phi / pi) + 0.5f;
      s = (theta / (pi * 2.0f)) + 0.5f;

      glTexCoord2f(s, t);
      glNormal3f(x, y, z);
      glVertex3f(x, y, z);

      x = cos(phi) * cos(theta + inctheta) * radius;
      y = cos(phi) * sin(theta + inctheta) * radius;
      z = sin(phi) * radius;
      t = (phi / pi) + 0.5f;
      s = ((theta + inctheta) / (pi * 2.0f)) + 0.5f;

      glTexCoord2f(s, t);
      glNormal3f(x, y, z);
      glVertex3f(x, y, z);

      x = cos(phi + incphi) * cos(theta + inctheta) * radius;
      y = cos(phi + incphi) * sin(theta + inctheta) * radius;
      z = sin(phi + incphi) * radius;
      t = ((phi + incphi) / pi) + 0.5f;
      s = ((theta + inctheta) / (pi * 2.0f)) + 0.5f;

      glTexCoord2f(s, t);
      glNormal3f(x, y, z);
      glVertex3f(x, y, z);

      x = cos(phi + incphi) * cos(theta) * radius;
      y = cos(phi + incphi) * sin(theta) * radius;
      z = sin(phi + incphi) * radius;
      t = ((phi + incphi) / pi) + 0.5f;
      s = (theta / (pi * 2.0f)) + 0.5f;

      glTexCoord2f(s, t);
      glNormal3f(x, y, z);
      glVertex3f(x, y, z);
    }
  }

  glEnd();
}

void Objects::drawAxes() {
  glMatrixMode(GL_MODELVIEW); // switch to modelview
  glPushMatrix();             // push a matrix on the stack

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, grey); // set colour
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, grey); // but they do emit

  glBegin(GL_LINES); // start drawing lines
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(1.0, 0.0, 0.0); // x-axis
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 1.0, 0.0); // y-axis
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, 1.0); // z-axis
  glEnd();

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
               black); // set reflective colour

  glPushMatrix();                 // push a matrix on the stack
  glTranslatef(0.0, 0.0, 1.0);    // move to end of z axis
  solidCone(0.02, 0.1, 32, 32);   // draw a solid cone for the z-axis
  glRotatef(90.0, 1.0, 0.0, 0.0); // rotate to draw in z=0 plane
  glTranslatef(0.1, 0.0, 0.0);    // move away slightly
  glScalef(0.001, 0.001, 0.001);  // scale down
  qGLWidget->renderText(0, 0, 0, "Z");
  glPopMatrix(); // pop matrix off stack

  glPushMatrix();              // push a matrix on the stack
  glTranslatef(0.0, 1.0, 0.0); // move to end of y axis
  glRotatef(-90.0, 1.0, 0.0, 0.0);
  solidCone(0.02, 0.1, 32, 32);   // draw a solid cone for the y-axis
  glRotatef(90.0, 1.0, 0.0, 0.0); // rotate to draw in y=0 plane
  glTranslatef(0.1, 0.0, 0.0);    // move away slightly
  glScalef(0.001, 0.001, 0.001);  // scale down
  qGLWidget->renderText(0, 0, 0, "Y");
  glPopMatrix(); // pop matrix off stack

  glPushMatrix();              // push a matrix on the stack
  glTranslatef(1.0, 0.0, 0.0); // move to end of x axis
  glRotatef(90.0, 0.0, 1.0, 0.0);
  solidCone(0.02, 0.1, 32, 32);   // draw a solid cone for the x-axis
  glRotatef(90.0, 0.0, 1.0, 0.0); // rotate to draw in x=0 plane
  glTranslatef(0.0, 0.1, 0.0);    // move away slightly
  glScalef(0.001, 0.001, 0.001);  // scale down
  qGLWidget->renderText(0, 0, 0, "X");
  glPopMatrix(); // pop matrix off stack

  glMatrixMode(GL_MODELVIEW); // switch to modelview
  glPopMatrix();              // pop matrix off stack
} // drawAxes()

// function to draw grid on z = 0 plane
void Objects::drawGrid(GLfloat *colour, int nGridlines) {
  int x, z;
  // edges don't reflect
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, black);
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, colour); // but they do emit
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  for (x = -nGridlines; x <= nGridlines; x++) { // for each x
    if (x % 12)
      glLineWidth(1.0);
    else
      glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3i(x, 0, -nGridlines);
    glVertex3i(x, 0, +nGridlines);
    glEnd();
  }                                             // for each x
  for (z = -nGridlines; z <= nGridlines; z++) { // for each y
    if (z % 12)
      glLineWidth(1.0);
    else
      glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex3i(-nGridlines, 0, z);
    glVertex3i(+nGridlines, 0, z);
    glEnd();
  } // for each y
  glLineWidth(1.0);
  glPopMatrix();
  // stop emitting, otherwise other objects will emit the same colour
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, black);
} // drawGrid()
