/*
 *
 * PPMImage.h
 * Simple 8-bit PPM based image class
 * stored in RGBA format, saved as ASCII RGB
 * Optimized for simplicity, not speed or memory
 *
 */

#ifndef PPM_IMAGE_H
#define PPM_IMAGE_H

#include "RGBValue.h"
#include <cstdio>
#include <cstdlib>
#include <iostream>

typedef unsigned char Byte;
typedef unsigned int Index;

class PPMImage {

private:
  Byte *_data;
  Index _width, _height; // dimensions in each direction

public:
  PPMImage();  // default constructor
  ~PPMImage(); // destructor

  Index width() { return _width; }
  Index height() { return _height; }

  // resizes the array - allows us to delay construction
  bool resize(Index width, Index height);

  void setPixel(Index row, Index col, Byte red, Byte green, Byte blue,
                Byte alpha = 255);
  void setPixel(Index row, Index col, RGBValue colour, Byte alpha = 255);

  bool readFile(std::string fileName);  // read from file
  bool writeFile(std::string fileName); // write to file

  void clear();
  void display(); // render to screen
};

#endif
