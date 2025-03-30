#ifndef IMAGE_TEST_HELPERS_HPP
#define IMAGE_TEST_HELPERS_HPP

/* Image_test_helpers.hpp
 * Originally written by James Juett at the University of Michigan
 * for the image processing project in EECS 280, Winter 2016.
 */

#include "Image.hpp"

// EFFECTS: Returns true if p1 and p2 are equal. Returns false otherwise.
bool Pixel_equal(Pixel p1, Pixel p2);

// REQUIRES: img1 points to a valid Image
//           img2 points to a valid Image
// EFFECTS:  Returns true if img1 and img2 are the same size and
//           contain exactly the same pixels. Returns false otherwise.
bool Image_equal(const Image* img1, const Image* img2);

#endif // IMAGE_TEST_HELPERS_HPP
