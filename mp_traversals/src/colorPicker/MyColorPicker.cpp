#include "cs225/HSLAPixel.h"
#include "../Point.h"

#include "ColorPicker.h"
#include "MyColorPicker.h"
#include <random>
#include <chrono>

using namespace cs225;

/**
 * Picks the color for pixel (x, y).
 * Using your own algorithm
 */
MyColorPicker::MyColorPicker() {
  hue = rand() % 360;
}

HSLAPixel MyColorPicker::getColor(unsigned x, unsigned y) {
  /* @todo [Part 3] */
  HSLAPixel pix = HSLAPixel();
  pix.h = hue;
  pix.s = (double)(rand() % x) / (double)x;
  pix.l = (double)(rand() % y) / (double)y;
  return pix;
}
