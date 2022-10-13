#include "cs225/PNG.h"
#include <list>
#include <iostream>
#include <utility>
#include <chrono>

#include "colorPicker/ColorPicker.h"
#include "imageTraversal/ImageTraversal.h"

#include "Point.h"
#include "Animation.h"
#include "FloodFilledImage.h"

using namespace cs225;

/**
 * Constructs a new instance of a FloodFilledImage with a image `png`.
 * 
 * @param png The starting image of a FloodFilledImage
 */
FloodFilledImage::FloodFilledImage(const PNG & png) {
  /** @todo [Part 2] */
  image_ = png;
}

/**
 * Adds a FloodFill operation to the FloodFillImage.  This function must store the operation,
 * which will be used by `animate`.
 * 
 * @param traversal ImageTraversal used for this FloodFill operation.
 * @param colorPicker ColorPicker used for this FloodFill operation.
 */
void FloodFilledImage::addFloodFill(ImageTraversal & traversal, ColorPicker & colorPicker) {
  /** @todo [Part 2] */
  std::pair<ImageTraversal&, ColorPicker&> floodFill(traversal, colorPicker);
  floodFills.push_back(floodFill);
}

/**
 * Creates an Animation of frames from the FloodFill operations added to this object.
 * 
 * Each FloodFill operation added by `addFloodFill` is executed based on the order
 * the operation was added.  This is done by:
 * 1. Visiting pixels within the image based on the order provided by the ImageTraversal iterator and
 * 2. Updating each pixel to a new color based on the ColorPicker
 * 
 * While applying the FloodFill to the image, an Animation is created by saving the image
 * after every `frameInterval` pixels are filled.  To ensure a smooth Animation, the first
 * frame is always the starting image and the final frame is always the finished image.
 * 
 * (For example, if `frameInterval` is `4` the frames are:
 *   - The initial frame
 *   - Then after the 4th pixel has been filled
 *   - Then after the 8th pixel has been filled
 *   - ...
 *   - The final frame, after all pixels have been filed)
 */ 
Animation FloodFilledImage::animate(unsigned frameInterval) const {
  Animation animation;
  /** @todo [Part 2] */
  PNG image = image_;
  for (std::pair<ImageTraversal&, ColorPicker&> floodFill : floodFills) {
    animation.addFrame(image);
    ImageTraversal::Iterator itr = floodFill.first.begin();
    auto end = floodFill.first.end();
    while (itr != end) {
      // auto start = std::chrono::steady_clock::now();
      for (unsigned i = 0; i < frameInterval && itr != end; i++) {

        Point point = *itr;
        HSLAPixel color = floodFill.second.getColor(point.x, point.y);

        HSLAPixel &pixel = image.getPixel(point.x, point.y);
        pixel = color;
        ++itr;
      }
      animation.addFrame(image);
      // auto end = std::chrono::steady_clock::now();
      // std::cout << std::chrono::duration<double, milli>(end - start).count() << std::endl;
    }
  }
  return animation;
}
