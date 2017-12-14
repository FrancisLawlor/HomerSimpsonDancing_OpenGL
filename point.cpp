#include "point.h"
#include <math.h>
#include <stdio.h>

// default constructor
Point::Point() { // constructor
  x = y = z = 0.0f;
  w = 1.0f;
} // constructor

// Constructor
Point::Point(float p_x, float p_y, float p_z) { // constructor
  x = p_x;
  y = p_y;
  z = p_z;
  w = 1.0;
} // constructor

// copy constructor
Point::Point(float p_x, float p_y, float p_z, float p_w) { // constructor
  x = p_x;
  y = p_y;
  z = p_z;
  w = p_w;
} // constructor

// Copy another Point & return self
Point &Point::operator=(Point point) { // operator =
  int i;
  for (i = 0; i < 4; i++)
    (*this)[i] = point[i];
  return (*this);
} // operator =

// Scalar right-multiplication
Point Point::operator*(float scaleFactor) { // operator *
  Point result;
  int i;
  for (i = 0; i < 4; i++)
    result[i] = (*this)[i] * scaleFactor;
  return result;
} // operator *

// Scalar right-division
Point Point::operator/(float scaleFactor) { // operator /
  Point result;
  int i;
  for (i = 0; i < 4; i++)
    result[i] = (*this)[i] / scaleFactor;
  return result;
} // operator /

// Add a vector to a point to get another point
// Does so in Cartesian space, but leaves w-coordinate the same
Point Point::operator+(Vector vector) { // operator +
  Point result;
  int i;
  // add the vector, multiplied by w
  for (i = 0; i < 4; i++)
    result[i] = (*this)[i] + vector[i] * w;
  return result;
} // operator +

// Negation (unary minus)
Point Point::operator-() { // operator -
  Point result;
  int i;
  // note that the w coordinate is unchanged
  // either that, or we could flip w instead
  for (i = 0; i < 3; i++)
    result[i] = -(*this)[i];
  return result;
} // operator -

// Subtract a vector from a point
Point Point::operator-(Vector vector) { // operator -
  Point result;
  int i;
  // subtract the vector, multiplied by w
  for (i = 0; i < 4; i++)
    result[i] = (*this)[i] - vector[i] * w;
  return result;
} // operator -

// Subtraction (returns a vector)
Vector Point::operator-(Point point) { // operator -
  Vector result;
  int i;
  for (i = 0; i < 3; i++)
    result[i] = (*this)[i] / w - point[i] / point.w;
  return result;
} // operator -

// Divide through by homogeneous coordinate (if a point)
Point Point::DeHomogenize() { // DeHomogenize()
  Point result;
  int i;
  for (i = 0; i < 3; i++)
    result[i] = (*this)[i] / w;
  w = 0.0;
  return result;
} // DeHomogenize()

float &Point::operator[](int i) {
  switch (i) {
  case 0:
    return x;
  case 1:
    return y;
  case 2:
    return z;
  case 3:
    return w;
  default:
    return w;
  }
}

// Scalar left-multiplication
Point operator*(Point point, float scaleFactor) { // operator *
  Point result;
  int i;
  for (i = 0; i < 4; i++)
    result[i] = point[i] * scaleFactor;
  return result;
} // operator *
