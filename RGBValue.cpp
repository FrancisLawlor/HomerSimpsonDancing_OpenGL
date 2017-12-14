/*
 *
 * RGBValue.cpp
 * Simple class to encapsulate RGB colour
 *
 */

#include "RGBValue.h"
#include <iostream>

// default constructor
RGBValue::RGBValue() { red = green = blue = 0; }

// value constructor
RGBValue::RGBValue(Byte Red, Byte Green, Byte Blue) {
  red = Red;
  green = Green;
  blue = Blue;
}

Byte &RGBValue::operator[](int i) {
  switch (i) {
  case 0:
    return red;
  case 1:
    return green;
  case 2:
    return blue;
  default:
    std::cout << "RGBValue::operator [](int i) - Operator out of bounds: " << i
              << std::endl;
    return red;
  }
}

RGBValue &RGBValue::operator+=(const RGBValue &color) {
  *this = *this + color;
  return *this;
}

// modulate with colour
RGBValue RGBValue::modulate(RGBValue &color) {
  // convert to float
  float u[3] = {(float)red / 255, (float)green / 255, (float)blue / 255};
  float v[3] = {(float)color.red / 255, (float)color.green / 255,
                (float)color.blue / 255};

  // return a new RGBValue
  return RGBValue((int)(255 * u[0] * v[0]), (int)(255 * u[1] * v[1]),
                  (int)(255 * u[2] * v[2]));
}

// Unary Minus
const RGBValue operator-(const RGBValue &right) {
  return RGBValue(-right.red, -right.green, -right.blue);
}

// Addition
const RGBValue operator+(const RGBValue &left, const RGBValue &right) {
  int red = left.red + right.red;
  int green = left.green + right.green;
  int blue = left.blue + right.blue;
  if (red > 255)
    red = 255;
  if (green > 255)
    green = 255;
  if (blue > 255)
    blue = 255;
  return RGBValue(red, green, blue);
}

// Subtraction
const RGBValue operator-(const RGBValue &left, const RGBValue &right) {
  int red = left.red - right.red;
  int green = left.green - right.green;
  int blue = left.blue - right.blue;
  if (red < 0)
    red = 0;
  if (green < 0)
    green = 0;
  if (blue < 0)
    blue = 0;
  return RGBValue(red, green, blue);
}

// Scalar Multiplication
const RGBValue operator*(float scalar, const RGBValue &colour) {
  return RGBValue(scalar * colour.red, scalar * colour.green,
                  scalar * colour.blue);
}

const RGBValue operator*(const RGBValue &colour, float scalar) {
  return RGBValue(scalar * colour.red, scalar * colour.green,
                  scalar * colour.blue);
}
