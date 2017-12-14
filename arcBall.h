#ifndef ARCBALL_H
#define ARCBALL_H

#include "quat.h"
#include "vector.h"

class arcBall {
public:
  arcBall();
  ~arcBall();
  Vector screenToPoint(int x, int y, int windowWidth, int windowHeight);
  void startBall(int x, int y, int windowWidth, int windowHeight);
  void endBall(int x, int y, int windowWidth, int windowHeight);
  void updateBall(int x, int y, int windowWidth, int windowHeight);
  void getMatrix(float *matrix);
  int min(int, int);
  void reset();

private:
  quat currQuat;
  Vector v0;
};
#endif
