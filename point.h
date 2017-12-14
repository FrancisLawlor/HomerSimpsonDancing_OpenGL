#ifndef Point_H
#define Point_H

class Point;

#include "vector.h"
#include <stdio.h>

// Assume that we carry EVERYTHING in explicit homogeneous coordinates
class Point {
public:
  float x, y, z, w;

  // default constructor
  Point();

  // constructor
  Point(float p_x, float p_y, float p_z);

  // copy constructor
  Point(float p_x, float p_y, float p_z, float p_w);

  // Copy another Point & return self
  Point &operator=(Point point);

  // Scalar right-multiplication
  Point operator*(float scaleFactor);

  // Scalar right-division
  Point operator/(float scaleFactor);

  // Add a vector to a point to get another point
  // Does so in Cartesian space, but leaves w-coordinate the same
  Point operator+(Vector vector);

  // Negation (unary minus)
  Point operator-();

  // Subtract a vector from a point
  Point operator-(Vector vector);

  // Subtraction (returns a vector)
  Vector operator-(Point point);

  // Divide through by homogeneous coordinate (if a point)
  Point DeHomogenize();

  // Returns the component from an index
  float &operator[](int);
};

// Scalar left-multiplication
Point operator*(Point point, float scaleFactor);

#endif
