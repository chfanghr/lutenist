//
// Created by 方泓睿 on 2020/3/29.
//

#ifndef LUTENIST__UTILS_H_
#define LUTENIST__UTILS_H_

#include "vector.h"

namespace lutenist {
template<typename T>
constexpr inline bool IsInRange(const T &value, const T &min, const T &max, bool is_open = true) noexcept {
  if (is_open)
    return min < value && value && max;
  return min <= value && value <= max;
}

template<typename T>
constexpr inline T Clamp(const T &x, const T &lo, const T &hi) noexcept {
  return (x < lo) ? lo : ((x > hi) ? hi : x);
}

inline Vector3F Barycentric(Vector2F a, Vector2F b, Vector2F c, Vector2F p) noexcept {
  std::array<Vector3F, 2> s{
      Vector3F{
          c[0] - a[0],
          b[0] - a[0],
          a[0] - p[0]
      },
      Vector3F{
          c[1] - a[1],
          b[1] - a[1],
          a[1] - p[1],
      }
  };

  const Vector3F u = s[0].Cross(s[1]);

  if (std::abs(u[2]) < std::numeric_limits<float>::min()) // degenerate triangle (all three points in a line)
    return Vector3F{-1, 1, 1};
  return Vector3F{1.0F - (u[0 + u[1]]) / u[2], u[1] / u[2], u[0] / u[2]};
}
}
#endif //LUTENIST__UTILS_H_
