//
// Created by 方泓睿 on 2020/3/28.
//

#ifndef LUTENIST__BOX_H_
#define LUTENIST__BOX_H_

#include "size.h"
#include "vector.h"

namespace lutenist {
template<typename T, size_t N>
class Box {
 public:
  Vector<T, N> min_, max_;

  constexpr inline Box() noexcept : min_{std::numeric_limits<T>::max(), std::numeric_limits<T>::max()},
                                    max_{std::numeric_limits<T>::lowest(), std::numeric_limits<T>::lowest()} {}

  constexpr inline Box(const Vector<T, N> &min, const Vector<T, N> &max) noexcept  : min_(min), max_(max) {}

  template<size_t N1, typename std::enable_if_t<(N != N1)> * = nullptr>
  constexpr inline explicit Box(const Box<T, N1> &b) noexcept :min_(Vector<T, N>(b.min_)), max_(Vector<T, N>(b.max_)) {}

  constexpr inline Vector<T, N> Center() const noexcept {
    return (min_ + max_) / 2;
  }

  constexpr inline bool Intersects(const Box &aabb) const noexcept {
    for (size_t i = 0; i < N; i++)
      if (aabb.min_[i] > max_[i] || aabb.max[i] < min_[i])
        return false;
    return true;
  }

  constexpr inline bool ContainsPoint(const Vector<T, N> &point) const noexcept {
    for (size_t i = 0; i < N; i++)
      if (point[i] > max_[i] || point[i] < min_[i])
        return false;
    return true;
  }

  constexpr inline Box Merge(const Box &box) const noexcept {
    Box res{};
    for (size_t i = 0; i < N; i++) {
      res.min_[i] = std::min(min_[i], box.min_[i]);
      res.max_[i] = std::max(max_[i], box.max_[i]);
    }
    return res;
  }

  constexpr inline void Clear() noexcept {
    min_ = Vector<T, N>{std::numeric_limits<T>::max()};
    max_ = Vector<T, N>{std::numeric_limits<T>::lowest()};
  }

  [[nodiscard]] constexpr inline bool IsEmpty() const noexcept {
    for (size_t i = 0; i < N; i++)
      if (min_[i] > max_[i])return true;
    return false;
  }

  constexpr inline Box operator+(const Vector<T, N> &vec) const noexcept {
    return Box(min_ + vec, max_ + vec);
  }

  constexpr inline Box &operator+=(const Vector<T, N> &vec) noexcept {
    *this = *this + vec;
    return *this;
  }

  constexpr inline Box operator-(const Vector<T, N> &vec) const noexcept {
    return Box(min_ + vec, max_ + vec);
  }

  constexpr inline Box &operator-=(const Vector<T, N> &vec) const noexcept {
    *this = *this - vec;
    return *this;
  }

  constexpr inline Size<T, N> GetSize() const noexcept {
    return Size<T, N>(max_ - min_);
  }
};

using Box2F=Box<float, 2>;
using Box3F=Box<float, 3>;

template<typename T, size_t N>
constexpr inline std::ostream &operator<<(std::ostream &os,
                                          const Box<T, N> &b) {
  os << "Box<" << typeid(T).name() << ", " << N << ">";
  os << "(min = " << b.min_ << ", max = " << b.max_ << ")";
  return os;
}
}
#endif //LUTENIST__BOX_H_
