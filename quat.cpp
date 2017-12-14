/*
 *  quat.cpp
 *
 */

#include "quat.h"
#include <cmath>
#include <iostream>

quat::quat() : real(1.0f), imag(Vector(0, 0, 0)) {}
quat::~quat() {}
quat::quat(float *matrix) {
  real = std::sqrt(1.0 + matrix[0] + matrix[5] + matrix[10]) / 2.0;
  float w4 = 4.0 * real;
  imag = Vector((matrix[6] - matrix[9]) / w4, (matrix[8] - matrix[2]) / w4,
                (matrix[1] - matrix[4]) / w4);
}
quat::quat(float w, float x, float y, float z) {
  imag.dx = x, imag.dy = y, imag.dz = z, real = w;
}
quat::quat(Vector v, float w) { imag = v, real = w; }
quat::quat(float w, Vector v) { imag = v, real = w; }
Vector quat::getImag() { return imag; }

float quat::getReal() { return real; }
quat quat::mult(quat q) {
  return quat(((real * q.getReal()) - (imag.dot(q.getImag()))),
              ((real * q.getImag()) + (q.getReal() * imag) +
               (imag.cross(q.getImag()))));
}

quat quat::conj() { return quat(real, -imag.dx, -imag.dy, -imag.dz); }

float quat::sumSquare() {
  return (real * real + imag.dx * imag.dx + imag.dy * imag.dy +
          imag.dz * imag.dz);
}

quat quat::inverse() {
  return quat(real / sumSquare(), conj().getImag() / sumSquare());
}

void quat::toMatrix(float *matrix) {
  float treal = real;
  matrix[0] = 1.0f - ((2.0f * imag.dy * imag.dy) + (2.0f * imag.dz * imag.dz));
  matrix[4] = 2.0f * ((imag.dx * imag.dy) - (imag.dz * treal));
  matrix[8] = 2.0f * ((imag.dx * imag.dz) + (imag.dy * treal));

  matrix[1] = 2.0f * ((imag.dx * imag.dy) + (imag.dz * treal));
  matrix[5] = 1.0f - ((2.0f * imag.dx * imag.dx) + (2.0f * imag.dz * imag.dz));
  matrix[9] = 2.0f * ((imag.dy * imag.dz) - (imag.dx * treal));

  matrix[2] = 2.0f * ((imag.dx * imag.dz) - (imag.dy * treal));
  matrix[6] = 2.0f * ((imag.dy * imag.dz) + (imag.dx * treal));
  matrix[10] = 1.0f - ((2.0f * imag.dy * imag.dy) + (2.0f * imag.dx * imag.dx));
}
