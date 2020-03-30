//
// Created by 方泓睿 on 2020/3/28.
//

#ifndef LUTENIST__CONVEX_VOLUME_H_
#define LUTENIST__CONVEX_VOLUME_H_

#include <vector>

#include "box.h"
#include "plane.h"

namespace lutenist {
template<typename T>
class ConvexVolume {
 public:
  std::vector<Plane<T>> planes_;

  constexpr inline Plane<T> &operator[](size_t index) noexcept {
    assert(index < planes_.size());
    return planes_[index];
  }

  constexpr inline Plane<T> operator[](size_t index) const noexcept {
    assert(index < planes_.size());
    return planes_[index];
  }

  template<typename... A>
  constexpr inline explicit ConvexVolume(const A &... args)
      : planes_{static_cast<T>(args)...} {}

  constexpr inline ConvexVolume() noexcept = default;

  constexpr inline explicit ConvexVolume(
      const std::vector<Plane<T>> &plane) noexcept
      : planes_(plane) {}

  template<size_t N, typename std::enable_if_t<(N >= 3)> * = nullptr>
  constexpr inline bool IsPointInside(const Vector<T, N> &position) const
  noexcept {
    return IsSphereInside(position, -T(0));
  }

  template<size_t N, typename std::enable_if_t<(N >= 3)> * = nullptr>
  constexpr inline bool IsSphereInside(const Vector<T, N> &position,
                                       const T &radius) const noexcept {
    for (const Plane<T> &plane : planes_)
      if (plane.Dot(position) < -radius)
        return false;
    return true;
  }

  constexpr inline bool IsBoxInside(const Box<T, 3> &box) const noexcept {
    const Vector<T, 4> left_bottom_back(box.min_[0], box.min_[1], box.min_[2],
                                        1);
    const Vector<T, 4> left_bottom_front(box.min_[0], box.min_[1], box.max_[2],
                                         1);
    const Vector<T, 4> left_top_back(box.min_[0], box.max_[1], box.min_[2], 1);
    const Vector<T, 4> left_top_front(box.min_[0], box.max_[1], box.max_[2], 1);
    const Vector<T, 4> right_bottom_back(box.max_[0], box.min_[1], box.min_[2],
                                         1);
    const Vector<T, 4> right_bottom_front(box.max_[0], box.min_[1], box.max_[2],
                                          1);
    const Vector<T, 4> right_top_back(box.max_[0], box.max_[1], box.min_[2], 1);
    const Vector<T, 4> right_top_front(box.max_[0], box.max_[1], box.max_[2],
                                       1);

    for (const Plane<T> &plane : planes_)
      if (plane.Dot(left_bottom_back) < T(0) &&
          plane.Dot(left_bottom_front) < T(0) &&
          plane.Dot(left_top_back) < T(0) && plane.Dot(left_top_front) < T(0) &&
          plane.Dot(right_bottom_back) < T(0) &&
          plane.Dot(right_bottom_front) < T(0) &&
          plane.Dot(right_top_back) < T(0) && plane.Dot(right_top_front) < T(0))
        return false;
    return true;
  }
};

template<typename T>
constexpr inline std::ostream &operator<<(std::ostream &os,
                                          const ConvexVolume<T> &cv) noexcept {
  os << "ConvexVolume<" << typeid(T).name() << ">(";

  bool is_first = true;

  for (const Plane<T> &plane : cv.planes_) {
    if (!is_first)
      os << ", ";
    else
      is_first = false;
    os << plane;
  }

  os << ")";
  return os;
}
}

#endif // LUTENIST__CONVEX_VOLUME_H_
