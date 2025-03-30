#ifndef JPEG_HPP
#define JPEG_HPP

/**
 * jpeg.hpp
 *
 * Provides functions that interface with libjpeg or libjpeg-turbo to
 * read or write jpeg files to or from Image objects.
 *
 * Author: Amir Kamil <akamil@umich.edu>
 * Date: 2024-03-30
 */

#include <cctype>
#include <cstdio>
#include <iostream>
#include <string>
#include "Image.hpp"

//////////////////////////////////////////////////////////////////////
// Interface
//////////////////////////////////////////////////////////////////////

// EFFECTS: Returns whether or not filename ends with .jpg or .jpeg,
//          ignoring capitalization.
bool has_jpeg_extension(const std::string &filename);

// REQUIRES: image points to an Image object
// MODIFIES: *image, cout
// EFFECTS:  Reads a data from the JPEG file specified by the given
//           filename into the given Image object.
//           Returns whether or not the file was successfully read.
bool read_jpeg(Image *image, const std::string &filename);

// REQUIRES: image points to a valid Image object
// MODIFIES: the file corresponding to the given filename, cout
// EFFECTS:  Writes the data in the given Image object into the file
//           of the given name, in JPEG format.
//           high_quality specifies whether to use low- or
//           high-quality settings for JPEG compression, which trades
//           off file size for image quality.
//           Returns whether or not the file was successfully written.
bool write_jpeg(const Image *image, const std::string &filename,
                bool high_quality=false);


//////////////////////////////////////////////////////////////////////
// Implementation
//////////////////////////////////////////////////////////////////////

#if JPEG_HPP_USE_LIBJPEG
  // if possible, check whether libjpeg is present
  #if defined(__has_include) && !__has_include(<jpeglib.h>)
    // libjpeg not found
    #error libjpeg not found. Set LIBJPEG_PATH in the Makefile to its path.
    #undef JPEG_HPP_USE_LIBJPEG
  #else
    // libjpeg confirmed or assumed to be present
    #include <jpeglib.h>
  #endif
#endif

// EFFECTS: Returns whether or not filename ends with .jpg or .jpeg,
//          ignoring capitalization.
bool has_jpeg_extension(const std::string &filename) {
  std::string extension = filename.substr(filename.rfind('.') + 1);
  for (auto &ch : extension) {
    ch = std::tolower(static_cast<unsigned char>(ch));
  }
  return extension == "jpg" || extension == "jpeg";
}

#if JPEG_HPP_USE_LIBJPEG

// REQUIRES: buffer refers to an array that is at least as large as
//           Image_width(image) if grayscale is true, or 3 *
//           Image_width(image) if it is false;
//           image points to a valid Image;
//           0 <= row < Image_height(image)
// MODIFIES: *image
// EFFECTS:  Copies the pixels from buffer into image at the given
//           row. If grayscale is true, then each element of buffer is
//           the value for all three colors of a pixel. Otherwise, the
//           buffer is assumed to hold color values in RGB "striped"
//           format (i.e. RGBRGB...).
template<typename Buffer>
static void copy_pixels(const Buffer &buffer, Image *image, int row,
                        bool grayscale) {
  for (int column = 0; column < Image_width(image); ++column) {
    Pixel pixel;
    if (grayscale) {
      pixel = {buffer[column], buffer[column], buffer[column]};
    } else {
      std::size_t base = 3 * column; // RGB format
      pixel = {buffer[base], buffer[base + 1], buffer[base + 2]};
    }
    Image_set_pixel(image, row, column, pixel);
  }
}

// REQUIRES: info_p points to a valid jpeg_decompress_struct, with the
//           header already read and decompression started;
//           image points to a valid Image of the appropriate size
// MODIFIES: *image
// EFFECTS:  Reads the data from the image source represented by
//           info_p into the given Image.
static void read_jpeg8(j_decompress_ptr info_p, Image *image) {
  int row_size = info_p->output_width * info_p->output_components;
  JSAMPARRAY buffer2d = (JSAMPARRAY)
    info_p->mem->alloc_sarray((j_common_ptr) info_p, JPOOL_IMAGE,
                              row_size, 1);
  auto buffer = buffer2d[0];
  while (info_p->output_scanline < info_p->output_height) {
    (void) jpeg_read_scanlines(info_p, buffer2d, 1);
    copy_pixels(buffer, image, info_p->output_scanline - 1,
                info_p->output_components == 1);
  }
}

// REQUIRES: image points to a valid Image of the appropriate size;
//           buffer refers to an array that is at least as large as
//           3 * Image_width(image);
//           0 <= row < Image_height(image)
// MODIFIES: the contents of buffer
// EFFECTS:  Reads a full row from image into buffer. Each color value
//           is truncated by the value of Mask.
template<int Mask, typename Buffer>
static void get_row(const Image *image, Buffer &buffer, int row) {
  for (int column = 0; column < Image_width(image); ++column) {
    Pixel pixel = Image_get_pixel(image, row, column);
    buffer[column * 3] = pixel.r & Mask;
    buffer[column * 3 + 1] = pixel.g & Mask;
    buffer[column * 3 + 2] = pixel.b & Mask;
  }
}

// REQUIRES: info_p points to a valid jpeg_compress_struct, with the
//           metadata already set and compression started;
//           image points to a valid Image of the appropriate size
// MODIFIES: the data target represented by info_p
// EFFECTS:  Writes the data from the given Image to the data target
//           represented by info_p.
static void write_jpeg8(j_compress_ptr info_p, const Image *image) {
  static constexpr int MASK = (1 << 8) - 1;
  int row_size = info_p->image_width * 3; // always RGB
  JSAMPARRAY buffer2d = (JSAMPARRAY)
    info_p->mem->alloc_sarray((j_common_ptr) info_p, JPOOL_IMAGE,
                              row_size, 1);
  auto buffer = buffer2d[0];
  while (info_p->next_scanline < info_p->image_height) {
    get_row<MASK>(image, buffer, info_p->next_scanline);
    jpeg_write_scanlines(info_p, buffer2d, 1);
  }
}

// REQUIRES: image points to an Image object
// MODIFIES: *image, cout
// EFFECTS:  Reads a data from the JPEG file specified by the given
//           filename into the given Image object.
//           Returns whether or not the file was successfully read.
bool read_jpeg(Image *image, const std::string &filename) {
  std::FILE *infile = std::fopen(filename.c_str(), "rb");
  if (!infile) {
    std::cout << "Failed to open " << filename << " for reading"
              << std::endl;
    return false;
  }

  struct jpeg_decompress_struct info;
  struct jpeg_error_mgr err;
  info.err = jpeg_std_error(&err);
  jpeg_create_decompress(&info);
  jpeg_stdio_src(&info, infile);
  if (jpeg_read_header(&info, true) != 1) { // did not succeed
    std::cout << "Failed to read jpeg header from " << filename
              << std::endl;
    jpeg_destroy_decompress(&info);
    return false;
  }

  jpeg_start_decompress(&info);
  Image_init(image, info.output_width, info.output_height);
  if (info.data_precision != 8) {
    std::cout << "Only 8-bit color depth is supported, but " << filename
              << " has " << info.data_precision << "-bit depth"
              << std::endl;
    jpeg_destroy_decompress(&info);
    return false;
  } else {
    read_jpeg8(&info, image);
  }

  jpeg_finish_decompress(&info);
  jpeg_destroy_decompress(&info);
  std::fclose(infile);
  return true;
}

// REQUIRES: image points to a valid Image object
// MODIFIES: the file corresponding to the given filename, cout
// EFFECTS:  Writes the data in the given Image object into the file
//           of the given name, in JPEG format.
//           high_quality specifies whether to use low- or
//           high-quality settings for JPEG compression, which trades
//           off file size for image quality.
//           Returns whether or not the file was successfully written.
bool write_jpeg(const Image *image, const std::string &filename,
                bool high_quality) {
  static constexpr int QUALITY_HIGH = 95;
  static constexpr int QUALITY_LOW = 30;

  std::FILE *outfile = std::fopen(filename.c_str(), "wb");
  if (!outfile) {
    std::cout << "Failed to open " << filename << " for writing"
              << std::endl;
    return false;
  }

  struct jpeg_compress_struct info;
  struct jpeg_error_mgr err;
  info.err = jpeg_std_error(&err);
  jpeg_create_compress(&info);
  jpeg_stdio_dest(&info, outfile);
  info.image_width = Image_width(image);
  info.image_height = Image_height(image);
  info.input_components = 3;
  info.in_color_space = JCS_RGB;
  info.data_precision = 8;
  jpeg_set_defaults(&info);
  jpeg_set_quality(&info, high_quality ? QUALITY_HIGH : QUALITY_LOW, true);
  if (high_quality) {
    // turn off chrominance subsampling
    info.comp_info[0].h_samp_factor = 1;
    info.comp_info[0].v_samp_factor = 1;
  }

  jpeg_start_compress(&info, true);
  write_jpeg8(&info, image);

  jpeg_finish_compress(&info);
  jpeg_destroy_compress(&info);
  std::fclose(outfile);
  return true;
}

#else // JPEG_HPP_USE_LIBJPEG

static constexpr const char *MISSING_LIBJPEG =
  "=========================================================================\n"
  "Compiled without jpeg support. Make sure that libjpeg or libjpeg-turbo is\n"
  "installed, then set the Makefile variables to the appropriate values.\n"
  "See the project specification for more details.\n"
  "=========================================================================";

// MODIFIES: cout
// EFFECTS:  Prints an error message with instructions of how to
//           compile with libjpeg/libjpeg-turbo and returns false.
bool read_jpeg(Image *, const std::string &) {
  std::cout << MISSING_LIBJPEG << std::endl;
  return false;
}

// MODIFIES: cout
// EFFECTS:  Prints an error message with instructions of how to
//           compile with libjpeg/libjpeg-turbo and returns false.
bool write_jpeg(const Image *, const std::string &, bool) {
  std::cout << MISSING_LIBJPEG << std::endl;
  return false;
}

#endif // JPEG_HPP_USE_LIBJPEG

#endif // JPEG_HPP
