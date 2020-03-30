//
// Created by 方泓睿 on 2020/3/29.
//

#ifndef LUTENIST__TEXTURE_H_
#define LUTENIST__TEXTURE_H_

#include <cstdlib>
#include <vector>
#include <stdexcept>
#include <map>
#include <functional>

#include "color.h"
#include "sampler.h"
#include "size.h"

namespace lutenist {
using DownSampler=std::function<void(const Size2 &size, size_t pitch, const uint8_t *src, uint8_t *dst)>;

class Texture {
 public:
  enum class PixelFormat { kR8, kA8, kRGBA8, kFloat32 };

  static constexpr float gamma_ = 2.2F;

  static constexpr inline size_t
  GetPixelSize(PixelFormat pixel_format) noexcept {
    switch (pixel_format) {
      case PixelFormat::kR8:
      case PixelFormat::kA8:return sizeof(uint8_t) * 1;
      case PixelFormat::kRGBA8:return sizeof(uint8_t) * 4;
      case PixelFormat::kFloat32:return sizeof(float);
      default:assert(false);
    }
  }

 private:
  PixelFormat pixel_format_ = PixelFormat::kA8;
  Size2 size_{};
  bool mip_maps_ = false;
  std::vector<std::vector<uint8_t>> levels_{};
 public:
  size_t min_lod_ = 0, max_lod_ = std::numeric_limits<size_t>::max();
  float lod_bias{};

 public:
  explicit Texture(PixelFormat pixel_format = PixelFormat::kRGBA8,
                   size_t width = 0, size_t height = 0, bool mip_maps = false);

  explicit Texture(PixelFormat pixel_format = PixelFormat::kRGBA8,
                   const Size2 &size = Size2(0, 0), bool mip_maps = false);

  void Resize(const Size2 &size);

  [[nodiscard]] inline PixelFormat GetPixelFormat() const noexcept { return pixel_format_; }

  [[nodiscard]] inline size_t GetWidth() const noexcept { return size_.Width(); }

  [[nodiscard]] inline size_t GetHeight() const noexcept { return size_.Height(); }

  [[nodiscard]] inline Size2 GetSize() const noexcept { return size_; }

  [[nodiscard]] inline size_t GetLevelCount() const noexcept { return levels_.size(); }

  [[nodiscard]] inline std::vector<uint8_t> &GetData(size_t level = 0) { return levels_[level]; }

  void SetLevel(size_t level, const std::vector<uint8_t> &buffer);

  [[nodiscard]] Color GetPixel(size_t level, const Vector<size_t, 2> &pos) const;

  void GenerateMipMaps();

  [[nodiscard]] Color Sample(const Sampler &sampler, const Vector2F &coord) const;

  [[nodiscard]] Color Sample(const Vector2F &) const { return Color{}; }

 private:
  static std::map<PixelFormat, DownSampler> down_samplers_;
};
}

#endif // LUTENIST__TEXTURE_H_
