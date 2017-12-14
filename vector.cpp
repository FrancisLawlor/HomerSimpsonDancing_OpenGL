#include "vector.h"
#include <math.h>
#include <stdio.h>

// default constructor
Vector::Vector() { // constructor
  dx = dy = dz = 0.0f;
} // constructor

// copy constructor
Vector::Vector(float v_dx, float v_dy, float v_dz) { // constructor
  dx = v_dx;
  dy = v_dy;
  dz = v_dz;
} // constructor

// Copy another Vector & return self
Vector &Vector::operator=(Vector vector) { // assignment
  for (int i = 0; i < 3; i++)
    (*this)[i] = vector[i];
  return (*this);
} // assignment

// Scalar right-multiplication
Vector Vector::operator*(float scaleFactor) { // Scalar right-multiplication
  Vector result;
  for (int i = 0; i < 3; i++)
    result[i] = (*this)[i] * scaleFactor;
  return result;
} // Scalar right-multiplication

// Scalar right-division
Vector Vector::operator/(float scaleFactor) { // Scalar right-division
  Vector result;
  for (int i = 0; i < 3; i++)
    result[i] = (*this)[i] / scaleFactor;
  return result;
} // Scalar right-division

// scalar left-multiplication
Vector operator*(float scaleFactor,
                 Vector vector) { // scalar left-multiplication
  Vector result;
  for (int i = 0; i < 3; i++)
    result[i] = scaleFactor * vector[i];
  return result;
} // scalar left-multiplication

// Computes the length (sqrt of sum of squares)
float Vector::norm() { // Length()
  return sqrt(dot(*this));
} // Length()

// Return the unit (normalized) vector
Vector Vector::normalised() { // Normalize()
  Vector result = (*this) / norm();
  return result;
} // Normalize()

// Add a vector to a vector to get another vector
// Does so in Cartesian space, but leaves w-coordinate the same
Vector Vector::operator+(Vector vector) { // addition
  Vector result;
  for (int i = 0; i < 3; i++)
    result[i] = (*this)[i] + vector[i];
  return result;
} // addition

// Add a point to a vector to get another point
// Does so in Cartesian space, but leaves w-coordinate the same
Point Vector::operator+(Point point) { // addition
  Point result;
  for (int i = 0; i < 3; i++)
    result[i] = (*this)[i] * point[3] + point[i];
  result[3] = point[3];
  return result;
} // addition

// Negation (unary minus)
Vector Vector::operator-() { // unary minus
  return (*this) * -1;
} // unary minus

// Subtraction (returns a vector)
Vector Vector::operator-(Vector vector) { // minus
  Vector result;
  for (int i = 0; i < 3; i++)
    result[i] = (*this)[i] - vector[i];
  return result;
} // minus

// Compute the cross-product (this x vector)
Vector Vector::cross(Vector vector) { // Cross()
  Vector result;
  result.dx = dy * vector.dz - dz * vector.dy;
  result.dy = dz * vector.dx - dx * vector.dz;
  result.dz = dx * vector.dy - dy * vector.dx;
  return result;
} // Cross()

// Dot product computes (this . vector)
float Vector::dot(Vector vector) { // Dot()
  return (dx * vector.dx + dy * vector.dy + dz * vector.dz);
} // Dot()

float &Vector::operator[](int i) {
  switch (i) {
  case 0:
    return dx;
  case 1:
    return dy;
  case 2:
    return dz;
  default:
    return dz;
  }
}
