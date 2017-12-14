#ifndef VECTOR_H
#define VECTOR_H

// forward declaration for mutually recursive inclusion
class Vector;

#include "point.h"
#include <stdio.h>

class Vector { // class Vector
public:
  float dx, dy, dz;

  // default constructor
  Vector();

  // copy constructor
  Vector(float v_dx, float v_dy, float v_dz);

  // Copy another Vector & return self
  Vector &operator=(Vector vector);

  // Scalar right-multiplication
  Vector operator*(float scaleFactor);

  // Scalar right-division
  Vector operator/(float scaleFactor);

  // Add a point to a vector to get another point
  // Does so in Cartesian space, but leaves w-coordinate the same
  Point operator+(Point point);

  // Add a vector to a vector to get another vector
  // Does so in Cartesian space, but leaves w-coordinate the same
  Vector operator+(Vector vector);

  // Negation (unary minus)
  Vector operator-();

  // Subtraction (returns a vector)
  Vector operator-(Vector vector);

  // Computes the length (sqrt of sum of squares)
  float norm();

  // Return the unit (normalized) vector
  Vector normalised();

  // Compute the cross-product (this x vector)
  Vector cross(Vector other);

  // Dot product computes (this . vector)
  float dot(Vector other);

  // Returns the component from an index
  float &operator[](int);
}; // class Vector

// scalar left-multiplication
Vector operator*(float scaleFactor, Vector vector);

#endif
