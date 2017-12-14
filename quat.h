#ifndef QUAT_H
#define QUAT_H

#include "vector.h"

class quat {
public:
  quat();
  quat(float *matrix);
  quat(float w, float x, float y, float z);
  quat(Vector v, float w);
  quat(float w, Vector v);
  quat mult(quat q);
  quat conj();
  float sumSquare();
  quat inverse();
  Vector getImag();
  float getReal();
  void toMatrix(float *inMat);
  ~quat();

private:
  float real;
  Vector imag;
};
#endif
