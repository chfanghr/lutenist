//
// Created by 方泓睿 on 2020/3/30.
//

#include <fstream>
#include <stdexcept>

#include <lutenist/demo/bmp.h>

demo::BMP::BMP(const std::string &filename) {
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

void demo::BMP::Save(const std::string &filename) {
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
