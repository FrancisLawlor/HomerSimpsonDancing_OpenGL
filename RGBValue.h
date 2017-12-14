/*
 *
 * RGBValue.h
 * Simple class to encapsulate RGB colour
 *
 */

#ifndef _HEADER_RGB_VALUE
#define _HEADER_RGB_VALUE

typedef unsigned char Byte;

class RGBValue {
public:
  // just a container for the three components
  Byte red, green, blue;

  // constructor / destructor
  RGBValue();
  RGBValue(Byte Red, Byte Green, Byte Blue);
  Byte &operator[](int i);

  RGBValue &operator+=(const RGBValue &color);

  // modulate with colour
  RGBValue modulate(RGBValue &color);
};

// pixel operations
const RGBValue operator-(const RGBValue &right);
const RGBValue operator+(const RGBValue &left, const RGBValue &right);
const RGBValue operator-(const RGBValue &left, const RGBValue &right);
const RGBValue operator*(float scalar, const RGBValue &colour);
const RGBValue operator*(const RGBValue &colour, float scalar);

#endif
