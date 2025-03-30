#ifndef IMAGE_HPP
#define IMAGE_HPP

/* Image.hpp
 * Originally written by James Juett at the University of Michigan
 * for the image processing project in EECS 280, Winter 2016.
 */

#include <iostream>
#include "Matrix.hpp"

// Representation of an RGB Pixel used for
// parameter passing and returns by the
// Image module. This is a POD type.
struct Pixel {
  int r;
  int g;
  int b;
};

const int MAX_INTENSITY = 255;

// Representation of 2D RGB image.
// Image objects may be copied.
struct Image {
  int width;
  int height;
  Matrix red_channel;
  Matrix green_channel;
  Matrix blue_channel;
};

// REQUIRES: img points to an Image
//           0 < width && 0 < height
// MODIFIES: *img
// EFFECTS:  Initializes the Image with the given width and height, with
//           all pixels initialized to RGB values of 0.
void Image_init(Image* img, int width, int height);

// REQUIRES: img points to an Image
//           is contains an image in PPM format without comments
//           (any kind of whitespace is ok)
// MODIFIES: *img, is
// EFFECTS:  Initializes the Image by reading in an image in PPM format
//           from the given input stream.
// NOTE:     See the project spec for a discussion of PPM format.
void Image_init(Image* img, std::istream& is);

// REQUIRES: img points to a valid Image
// MODIFIES: os
// EFFECTS:  Writes the image to the given output stream in PPM format.
//           You must use the kind of whitespace specified here.
//           First, prints out the header for the image like this:
//             P3 [newline]
//             WIDTH [space] HEIGHT [newline]
//             255 [newline]
//           Next, prints out the rows of the image, each followed by a
//           newline. Each pixel in a row is printed as three ints
//           for its red, green, and blue components, in that order. Each
//           int is followed by a space. This means that there will be an
//           "extra" space at the end of each line. See the project spec
//           for an example.
void Image_print(const Image* img, std::ostream& os);

// REQUIRES: img points to a valid Image
// EFFECTS:  Returns the width of the Image.
int Image_width(const Image* img);

// REQUIRES: img points to a valid Image
// EFFECTS:  Returns the height of the Image.
int Image_height(const Image* img);

// REQUIRES: img points to a valid Image
//           0 <= row && row < Image_height(img)
//           0 <= column && column < Image_width(img)
// EFFECTS:  Returns the pixel in the Image at the given row and column.
Pixel Image_get_pixel(const Image* img, int row, int column);

// REQUIRES: img points to a valid Image
//           0 <= row && row < Image_height(img)
//           0 <= column && column < Image_width(img)
// MODIFIES: *img
// EFFECTS:  Sets the pixel in the Image at the given row and column
//           to the given color.
void Image_set_pixel(Image* img, int row, int column, Pixel color);

// REQUIRES: img points to a valid Image
// MODIFIES: *img
// EFFECTS:  Sets each pixel in the image to the given color.
void Image_fill(Image* img, Pixel color);

#endif // IMAGE_HPP
