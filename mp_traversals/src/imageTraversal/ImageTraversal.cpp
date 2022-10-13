#include <cmath>
#include <iterator>
#include <iostream>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"

/**
 * Calculates a metric for the difference between two pixels, used to
 * calculate if a pixel is within a tolerance.
 *
 * @param p1 First pixel
 * @param p2 Second pixel
 * @return the difference between two HSLAPixels
 */
double ImageTraversal::calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
  double h = fabs(p1.h - p2.h);
  double s = p1.s - p2.s;
  double l = p1.l - p2.l;

  // Handle the case where we found the bigger angle between two hues:
  if (h > 180) { h = 360 - h; }
  h /= 360;

  return sqrt( (h*h) + (s*s) + (l*l) );
}

/**
 * Default iterator constructor.
 */
ImageTraversal::Iterator::Iterator() {
  /** @todo [Part 1] */
}

ImageTraversal::Iterator::Iterator(std::unique_ptr<ImageTraversal> setTraversal, const PNG & png, const Point & start, double tolerance) : 
tol(tolerance), image(png) {
  /** @todo [Part 1] */
  traversal = move(setTraversal);
  int width = image.width();
  int height = image.height();
  visited = std::vector<std::vector<bool>>(width, std::vector<bool>(height, false));
  visited[start.x][start.y] = true;
  pix = image.getPixel(start.x, start.y);
}

/**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
  /** @todo [Part 1] */
  Point point = traversal.get()->pop();

  HSLAPixel p1 = image.getPixel(point.x, point.y);
  visited[point.x][point.y] = true;

  if (point.x + 1 < image.width()) { // Right
    if (!visited[point.x + 1][point.y] && calculateDelta(pix, image.getPixel(point.x + 1, point.y)) < tol) {
      traversal.get()->add(Point(point.x + 1, point.y));
    }
  }
  if (point.y + 1 < image.height()) { // Down
    if (!visited[point.x][point.y + 1] && calculateDelta(pix, image.getPixel(point.x, point.y + 1)) < tol) {
      traversal.get()->add(Point(point.x, point.y + 1));
    }
  }
  if (point.x >= 1) { // Left
    if (!visited[point.x - 1][point.y] && calculateDelta(pix, image.getPixel(point.x - 1, point.y)) < tol) {
      traversal.get()->add(Point(point.x - 1, point.y));
    }
  }
  if (point.y >= 1) { // Up
    if (!visited[point.x][point.y - 1] && calculateDelta(pix, image.getPixel(point.x, point.y - 1)) < tol) {
      traversal.get()->add(Point(point.x, point.y - 1));
    }
  }

  while (!traversal->empty() && visited[traversal->peek().x][traversal.get()->peek().y]) {
    traversal.get()->pop();
  }

  return *this;
}

/**
 * Iterator accessor opreator.
 *
 * Accesses the current Point in the ImageTraversal.
 */
Point ImageTraversal::Iterator::operator*() {
  /** @todo [Part 1] */
  return traversal->peek();
}

/**
 * Iterator inequality operator.
 *
 * Determines if two iterators are not equal.
 */
bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) const {
  /** @todo [Part 1] */
  if (traversal.get()->empty() == other.traversal.get()->empty() && tol == other.tol && image == other.image) {
    return false;
  }
  return true;
}

