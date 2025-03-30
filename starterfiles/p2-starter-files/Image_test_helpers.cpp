
/* Image_test_helpers.cpp
 * Originally written by James Juett at the University of Michigan
 * for the image processing project in EECS 280, Winter 2016.
 */

#include "Image_test_helpers.hpp"

bool Pixel_equal(Pixel p1, Pixel p2){
  return p1.r == p2.r && p1.g == p2.g && p1.b == p2.b;
}

bool Image_equal(const Image* img1, const Image* img2){
  if (Image_width(img1) != Image_width(img2)){ return false; }
  if (Image_height(img1) != Image_height(img2)){ return false; }

  for(int r = 0; r < Image_height(img1); ++r){
    for(int c = 0; c < Image_width(img1); ++c){
      if (!Pixel_equal(Image_get_pixel(img1, r, c), Image_get_pixel(img2, r, c))){
        return false;
      }
    }
  }
  
  return true;
}
