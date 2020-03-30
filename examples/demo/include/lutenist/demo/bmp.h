//
//  SoftwareRenderer
//

#ifndef LUTENIST_EXAMPLES_DEMO_BMP_H
#define LUTENIST_EXAMPLES_DEMO_BMP_H

#include <cmath>
#include <string>
#include <vector>

namespace demo {
class BMP {
 public:
  static constexpr int kBitmapFileHeaderBM = 0x4D42;

  struct BitmapFileHeader              /**** BMP file header structure ****/
  {
    unsigned short bf_type;           /* Magic number for file */
    unsigned int bf_size;           /* Size of file */
    unsigned short bf_reserved_1;      /* Reserved */
    unsigned short bf_reserved_2;      /* ... */
    unsigned int bf_offBits;        /* Offset to bitmap data_ */
  };

  struct BitmapInfoHeader              /**** BMP file info structure ****/
  {
    unsigned int bi_size;           /* Size of info header */
    int bi_width;          /* Width of image */
    int bi_height;         /* Height of image */
    unsigned short bi_planes;         /* Number of color planes */
    unsigned short bi_bitCount;       /* Number of bits per pixel */
    unsigned int bi_compression;    /* Type of compression to use */
    unsigned int bi_size_image;      /* Size of image data_ */
    int bi_x_pels_per_meter;  /* X pixels per meter */
    int bi_y_pels_per_meter;  /* Y pixels per meter */
    unsigned int bi_clr_used;        /* Number of colors used */
    unsigned int bi_clr_important;   /* Number of important colors */
  };

  struct RGBQuad                      /**** Colormap entry structure ****/
  {
    unsigned char rgb_blue;          /* Blue value */
    unsigned char rgb_green;         /* Green value */
    unsigned char rgb_red;           /* Red value */
    unsigned char rgb_reserved;      /* Reserved */
  };

  enum Compression {
    kRGB = 0x0000,
    kRLE8 = 0x0001,
    kRLE4 = 0x0002,
    kBITFIELDS = 0x0003,
    kJPEG = 0x0004,
    kPNG = 0x0005,
    kCMYK = 0x000B,
    kCMYKRLE8 = 0x000C,
    kCMYKRLE4 = 0x000D
  };

  BMP() = default;

  explicit BMP(const std::string &filename);

  [[nodiscard]] inline std::uint32_t GetWidth() const { return width_; }
  [[nodiscard]] inline std::uint32_t GetHeight() const { return height_; }

  [[nodiscard]] const std::vector<std::uint8_t> &GetData() const {
    return data_;
  }

  void SetData(std::uint32_t new_Width,
               std::uint32_t new_Height,
               const std::vector<std::uint8_t> &new_Data) {
    width_ = new_Width;
    height_ = new_Height;
    data_ = new_Data;
  }

  void Save(const std::string &filename);

 private:
  std::uint32_t width_ = 0;
  std::uint32_t height_ = 0;
  std::vector<std::uint8_t> data_;
};
}

#endif
