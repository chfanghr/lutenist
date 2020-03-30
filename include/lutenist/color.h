//
// Created by 方泓睿 on 2020/3/29.
//

#ifndef LUTENIST__COLOR_H_
#define LUTENIST__COLOR_H_

#include "vector.h"
#include "utils.h"

namespace lutenist {
class Color : public Vector4F {
 public:
  constexpr inline Color() noexcept = default;
  constexpr inline Color(const Color &) noexcept = default;

  constexpr inline explicit Color(uint32_t color) noexcept :
      Vector4F{
          float(static_cast<uint8_t>((color & 0xFF000000) >> 24)) / 255.0F, // NOLINT(hicpp-signed-bitwise)
          float(static_cast<uint8_t>((color & 0x00FF0000) >> 16)) / 255.0F, // NOLINT(hicpp-signed-bitwise)
          float(static_cast<uint8_t>((color & 0x0000FF00) >> 8)) / 255.0F, // NOLINT(hicpp-signed-bitwise)
          float(static_cast<uint8_t>((color & 0x000000FF))) / 255.0F, // NOLINT(hicpp-signed-bitwise)
      } {}

  constexpr inline Color &operator=(uint32_t color) noexcept {
    Vector4F::data_ = {
        float(static_cast<uint8_t>((color & 0xFF000000) >> 24)) / 255.0F, // NOLINT(hicpp-signed-bitwise)
        float(static_cast<uint8_t>((color & 0x00FF0000) >> 16)) / 255.0F, // NOLINT(hicpp-signed-bitwise)
        float(static_cast<uint8_t>((color & 0x0000FF00) >> 8)) / 255.0F, // NOLINT(hicpp-signed-bitwise)
        float(static_cast<uint8_t>((color & 0x000000FF))) / 255.0F, // NOLINT(hicpp-signed-bitwise)
    };

    return *this;
  }

  constexpr inline float &R() noexcept { return Vector4F::X(); }
  constexpr inline float &G() noexcept { return Vector4F::Y(); }
  constexpr inline float &B() noexcept { return Vector4F::Z(); }
  constexpr inline float &A() noexcept { return Vector4F::W(); }

  [[nodiscard]] constexpr inline float R() const noexcept { return Vector4F::X(); }
  [[nodiscard]] constexpr inline float G() const noexcept { return Vector4F::Y(); }
  [[nodiscard]] constexpr inline float B() const noexcept { return Vector4F::Z(); }
  [[nodiscard]] constexpr inline float A() const noexcept { return Vector4F::W(); }

  constexpr inline Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept :
      Vector4F{
          float(r) / 255.0F,
          float(g) / 255.0F,
          float(b) / 255.0F,
          float(a) / 255.0F,
      } {}

  constexpr inline Color(const Vector<uint8_t, 4> &color) noexcept :
      Color(color.X(), color.Y(), color.Z(), color.W()) {}

  constexpr inline Color(float r, float g, float b, float a) noexcept :
      Vector4F{
          r, g, b, a
      } {}

  constexpr inline Color(const Vector4F &color) noexcept : Vector4F(color) {}

  constexpr inline uint32_t GetIntValue() const noexcept {
    return (static_cast<uint32_t>(R() * 255.0F) << 24) |
        (static_cast<uint32_t>(G() * 255.0F) << 16) |
        (static_cast<uint32_t>(B() * 255.0F) << 8) |
        (static_cast<uint32_t>(A() * 255.0F));
  }

  inline uint32_t GetIntValueRaw() const noexcept {
    uint32_t res{};
    uint8_t *bytes = reinterpret_cast<uint8_t *>(&res);
    for (size_t i = 0; i < 4; i++)
      bytes[i] = static_cast<uint8_t>(Vector4F::data_[i] * 255.0F);
    return res;
  }
};

inline std::ostream &operator<<(std::ostream &os, const Color &color) noexcept {
  os << "Color"
     << "(R=" << uint32_t(color.R() * 255.0F)
     << ", G=" << uint32_t(color.G() * 255.0F)
     << ", B=" << uint32_t(color.B() * 255.0F)
     << ", A=" << uint32_t(color.A() * 255.0F)
     << ")";
  return os;
}
}

#endif //LUTENIST__COLOR_H_
