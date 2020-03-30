//
//  SoftwareRenderer
//

#ifndef LUTENIST_EXAMPLES_DEMO_BMP_H
#define LUTENIST_EXAMPLES_DEMO_BMP_H

#include <cmath>
#include <fstream>
#include <stdexcept>
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

  explicit BMP(const std::string &filename) {
    std::ifstream f(filename, std::ios::binary);

    BitmapFileHeader header{};

    f.read(reinterpret_cast<char *>(&header.bf_type), sizeof(header.bf_type));

    if (header.bf_type != kBitmapFileHeaderBM)
      throw std::runtime_error("Bad bitmap file");

    f.read(reinterpret_cast<char *>(&header.bf_size), sizeof(header.bf_size));
    f.read(reinterpret_cast<char *>(&header.bf_reserved_1), sizeof(header.bf_reserved_1));
    f.read(reinterpret_cast<char *>(&header.bf_reserved_2), sizeof(header.bf_reserved_2));
    f.read(reinterpret_cast<char *>(&header.bf_offBits), sizeof(header.bf_offBits));

    BitmapInfoHeader infoHeader{};
    f.read(reinterpret_cast<char *>(&infoHeader.bi_size), sizeof(infoHeader.bi_size));

    std::uint32_t offset = 0;

    if (offset + sizeof(infoHeader.bi_width) < infoHeader.bi_size) {
      f.read(reinterpret_cast<char *>(&infoHeader.bi_width), sizeof(infoHeader.bi_width));
      offset += sizeof(infoHeader.bi_width);
    }

    if (offset + sizeof(infoHeader.bi_height) < infoHeader.bi_size) {
      f.read(reinterpret_cast<char *>(&infoHeader.bi_height), sizeof(infoHeader.bi_height));
      offset += sizeof(infoHeader.bi_height);
    }

    if (offset + sizeof(infoHeader.bi_planes) < infoHeader.bi_size) {
      f.read(reinterpret_cast<char *>(&infoHeader.bi_planes), sizeof(infoHeader.bi_planes));
      offset += sizeof(infoHeader.bi_planes);
    }

    if (offset + sizeof(infoHeader.bi_bitCount) < infoHeader.bi_size) {
      f.read(reinterpret_cast<char *>(&infoHeader.bi_bitCount), sizeof(infoHeader.bi_bitCount));
      offset += sizeof(infoHeader.bi_bitCount);
    }

    if (offset + sizeof(infoHeader.bi_compression) < infoHeader.bi_size) {
      f.read(reinterpret_cast<char *>(&infoHeader.bi_compression), sizeof(infoHeader.bi_compression));
      offset += sizeof(infoHeader.bi_compression);
    }

    if (infoHeader.bi_compression != kRGB)
      throw std::runtime_error("Compression not supported");

    if (offset + sizeof(infoHeader.bi_size_image) < infoHeader.bi_size) {
      f.read(reinterpret_cast<char *>(&infoHeader.bi_size_image), sizeof(infoHeader.bi_size_image));
      offset += sizeof(infoHeader.bi_size_image);
    }

    if (offset + sizeof(infoHeader.bi_x_pels_per_meter) < infoHeader.bi_size) {
      f.read(reinterpret_cast<char *>(&infoHeader.bi_x_pels_per_meter), sizeof(infoHeader.bi_x_pels_per_meter));
      offset += sizeof(infoHeader.bi_x_pels_per_meter);
    }

    if (offset + sizeof(infoHeader.bi_y_pels_per_meter) < infoHeader.bi_size) {
      f.read(reinterpret_cast<char *>(&infoHeader.bi_y_pels_per_meter), sizeof(infoHeader.bi_y_pels_per_meter));
      offset += sizeof(infoHeader.bi_y_pels_per_meter);
    }

    if (offset + sizeof(infoHeader.bi_clr_used) < infoHeader.bi_size) {
      f.read(reinterpret_cast<char *>(&infoHeader.bi_clr_used), sizeof(infoHeader.bi_clr_used));
      offset += sizeof(infoHeader.bi_clr_used);
    }

    if (offset + sizeof(infoHeader.bi_clr_important) < infoHeader.bi_size) {
      f.read(reinterpret_cast<char *>(&infoHeader.bi_clr_important), sizeof(infoHeader.bi_clr_important));
      offset += sizeof(infoHeader.bi_clr_important);
    }

    f.seekg(header.bf_offBits, std::ios::beg);

    data_.resize(infoHeader.bi_width * std::abs(infoHeader.bi_height) * sizeof(RGBQuad));
    std::fill(data_.begin(), data_.end(), 255);

    if (infoHeader.bi_height > 0) // bottom to top
    {
      for (int y = infoHeader.bi_height - 1; y >= 0; --y)
        for (int x = 0; x < infoHeader.bi_width; ++x)
          f.read(reinterpret_cast<char *>(&data_[(y * infoHeader.bi_width + x) * sizeof(RGBQuad)]),
                 infoHeader.bi_bitCount / 8);
    } else // top to bottom
    {
      for (int y = 0; y < std::abs(infoHeader.bi_height); ++y)
        for (int x = 0; x < infoHeader.bi_width; ++x)
          f.read(reinterpret_cast<char *>(&data_[(y * infoHeader.bi_width + x) * sizeof(RGBQuad)]),
                 infoHeader.bi_bitCount / 8);
    }

    width_ = static_cast<std::uint32_t>(infoHeader.bi_width);
    height_ = static_cast<std::uint32_t>(std::abs(infoHeader.bi_height));
  }

  inline std::uint32_t GetWidth() const { return width_; }
  inline std::uint32_t GetHeight() const { return height_; }

  const std::vector<std::uint8_t> &GetData() const {
    return data_;
  }

  void SetData(std::uint32_t newWidth,
               std::uint32_t newHeight,
               const std::vector<std::uint8_t> &newData) {
    width_ = newWidth;
    height_ = newHeight;
    data_ = newData;
  }

  void Save(const std::string &filename) {
    std::ofstream f(filename, std::ios::binary | std::ios::trunc);

    BitmapFileHeader header{};
    header.bf_type = kBitmapFileHeaderBM;
    header.bf_size = width_ * height_ * 4 + 14 + 40;
    header.bf_reserved_1 = 0;
    header.bf_reserved_2 = 0;
    header.bf_offBits = 14 + 40; // size of BITMAPFILEHEADER + size of BITMAPINFOHEADER

    f.write(reinterpret_cast<const char *>(&header.bf_type), sizeof(header.bf_type));
    f.write(reinterpret_cast<const char *>(&header.bf_size), sizeof(header.bf_size));
    f.write(reinterpret_cast<const char *>(&header.bf_reserved_1), sizeof(header.bf_reserved_1));
    f.write(reinterpret_cast<const char *>(&header.bf_reserved_2), sizeof(header.bf_reserved_2));
    f.write(reinterpret_cast<const char *>(&header.bf_offBits), sizeof(header.bf_offBits));

    BitmapInfoHeader infoHeader;
    infoHeader.bi_size = 40;
    infoHeader.bi_width = static_cast<int>(width_);
    infoHeader.bi_height = -static_cast<int>(height_);
    infoHeader.bi_planes = 1;
    infoHeader.bi_bitCount = 32;
    infoHeader.bi_compression = 0;
    infoHeader.bi_size_image = 0;
    infoHeader.bi_x_pels_per_meter = 0;
    infoHeader.bi_y_pels_per_meter = 0;
    infoHeader.bi_clr_used = 0;
    infoHeader.bi_clr_important = 0;

    f.write(reinterpret_cast<const char *>(&infoHeader.bi_size), sizeof(infoHeader.bi_size));
    f.write(reinterpret_cast<const char *>(&infoHeader.bi_width), sizeof(infoHeader.bi_width));
    f.write(reinterpret_cast<const char *>(&infoHeader.bi_height), sizeof(infoHeader.bi_height));
    f.write(reinterpret_cast<const char *>(&infoHeader.bi_planes), sizeof(infoHeader.bi_planes));
    f.write(reinterpret_cast<const char *>(&infoHeader.bi_bitCount), sizeof(infoHeader.bi_bitCount));
    f.write(reinterpret_cast<const char *>(&infoHeader.bi_compression), sizeof(infoHeader.bi_compression));
    f.write(reinterpret_cast<const char *>(&infoHeader.bi_size_image), sizeof(infoHeader.bi_size_image));
    f.write(reinterpret_cast<const char *>(&infoHeader.bi_x_pels_per_meter), sizeof(infoHeader.bi_x_pels_per_meter));
    f.write(reinterpret_cast<const char *>(&infoHeader.bi_y_pels_per_meter), sizeof(infoHeader.bi_y_pels_per_meter));
    f.write(reinterpret_cast<const char *>(&infoHeader.bi_clr_used), sizeof(infoHeader.bi_clr_used));
    f.write(reinterpret_cast<const char *>(&infoHeader.bi_clr_important), sizeof(infoHeader.bi_clr_important));

    f.write(reinterpret_cast<const char *>(data_.data()), static_cast<std::uint32_t>(data_.size()));
  }

 private:
  std::uint32_t width_ = 0;
  std::uint32_t height_ = 0;
  std::vector<std::uint8_t> data_;
};
}

#endif
