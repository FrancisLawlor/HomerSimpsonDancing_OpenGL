#include "arcBall.h"
#include <cmath>

arcBall::arcBall() {
  currQuat = quat();
  v0 = Vector();
}

arcBall::~arcBall() {}

/*
 * Converts screen coordinates to a point on a unit sphere needs to be modified
 * based on what click coordinate system is being used
 */
Vector arcBall::screenToPoint(int x, int y, int windowWidth, int windowHeight) {
  float retX, retY, s;
  float radius = static_cast<float>(min(windowWidth, windowHeight)) / 2.0f;
  retX = (x - (windowWidth / 2.0f)) / radius;
  // retY is negative because qt does funny click coordinates
  retY = -((y - (windowHeight / 2.0f)) / radius);

  float r = (retX * retX) + (retY * retY);
  float srR = std::sqrt(r);

  if (r >= 1.0f) {
    s = 1.0f / srR;
    return Vector(s * retX, s * retY, 0.0);
  } else {
    return Vector(retX, retY, std::sqrt(1.0f - r));
  }
}

void arcBall::startBall(int x, int y, int windowWidth, int windowHeight) {
  v0 = screenToPoint(x, y, windowWidth, windowHeight);
}

/*
 * Uses ken shoemakes tecnique
 */
void arcBall::updateBall(int x, int y, int windowWidth, int windowHeight) {
  Vector v1 = screenToPoint(x, y, windowWidth, windowHeight);
  Vector im = v0.cross(v1);
  float re = v0.dot(v1);
  quat newQuat = quat(re, im);
  currQuat = newQuat.mult(currQuat);
  v0 = v1;
}

/*
 * Takes current modelview matrix as 16 item array and replaces the rotation
 * part with the current rotation
 */
void arcBall::getMatrix(float *matrix) { currQuat.toMatrix(matrix); }

int arcBall::min(int i, int j) {
  if (i > j)
    return j;
  else
    return i;
}

void arcBall::reset() { currQuat = quat(); }
