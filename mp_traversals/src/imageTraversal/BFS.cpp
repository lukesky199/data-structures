#include <iterator>
#include <cmath>

#include <list>
#include <queue>
#include <stack>
#include <vector>
#include <chrono>

#include "cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"
#include "BFS.h"

using namespace cs225;

/**
 * Initializes a breadth-first ImageTraversal on a given `png` image,
 * starting at `start`, and with a given `tolerance`.
 * @param png The image this BFS is going to traverse
 * @param start The start point of this BFS
 * @param tolerance If the current point is too different (difference larger than tolerance) with the start point,
 * it will not be included in this BFS
 */
BFS::BFS(const PNG & png, const Point & start, double tolerance) {  
  /** @todo [Part 1] */
  image = png;
  startPoint = start;
  tol = tolerance;
  add(start);
}

BFS::BFS(const PNG & png, const Point & start, double tolerance, bool noStart) {  
  /** @todo [Part 1] */
  image = png;
  startPoint = start;
  tol = tolerance;
  if (noStart)
    add(start);
}

/**
 * Returns an iterator for the traversal starting at the first point.
 */
ImageTraversal::Iterator BFS::begin() {
  /** @todo [Part 1] */
  std::unique_ptr<ImageTraversal> temp = std::make_unique<BFS>(BFS(image, startPoint, tol));
  return ImageTraversal::Iterator(move(temp), image, startPoint, tol);
}

/**
 * Returns an iterator for the traversal one past the end of the traversal.
 */
ImageTraversal::Iterator BFS::end() {
  /** @todo [Part 1] */
  auto start = std::chrono::steady_clock::now();
  std::unique_ptr<ImageTraversal> temp = std::make_unique<BFS>(BFS(image, startPoint, tol));
  while (!temp.get()->empty()) {
    temp.get()->pop();
  }
  return ImageTraversal::Iterator(move(temp), image, startPoint, tol);
  auto end = std::chrono::steady_clock::now();
  std::cout << std::chrono::duration<double, std::milli>(end - start).count() << std::endl;
}

/**
 * Adds a Point for the traversal to visit at some point in the future.
 */
void BFS::add(const Point & point) {
  /** @todo [Part 1] */
  queue.push(point);
}

/**
 * Removes and returns the current Point in the traversal.
 */
Point BFS::pop() {
  /** @todo [Part 1] */
  Point popped(queue.front().x, queue.front().y);
  queue.pop();
  return popped;
}

/**
 * Returns the current Point in the traversal.
 */
Point BFS::peek() const {
  /** @todo [Part 1] */
  return Point(queue.front().x, queue.front().y);
}

/**
 * Returns true if the traversal is empty.
 */
bool BFS::empty() const {
  /** @todo [Part 1] */
  return queue.empty();
}
