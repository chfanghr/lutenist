//
// Created by 方泓睿 on 2020/3/29.
//

#ifndef LUTENIST__RECT_H_
#define LUTENIST__RECT_H_

#include <tuple>

#include "vector.h"
#include "size.h"

namespace lutenist {
template<typename T>
class Rect {
 public:
  Vector<T, 2> position_;
  Size<T, 2> size_;

  constexpr inline Rect() noexcept = default;

  constexpr inline Rect(const T &width, const T &height) noexcept: size_(width, height) {}

  constexpr inline Rect(const T &x, const T &y, const T &width, const T &height) : position_(x, y),
                                                                                   size_(width, height) {}

  constexpr inline Rect(const Vector<T, 2> &position, const Vector<T, 2> &size) noexcept
      : position_(position), size_(size) {}

  [[nodiscard]] constexpr inline bool IsEmpty() const noexcept {
    return size_.IsZero();
  }

  constexpr inline void SetPosition(const Vector<T, 2> new_position) noexcept {
    position_ = new_position;
  }

  constexpr inline T Left() const noexcept {
    return position_.X();
  }

  constexpr inline T Bottom() const noexcept {
    return position_.Y();
  }

  constexpr inline T Right() const noexcept {
    return position_.X() + size_.X();
  }

  constexpr inline T Top() const noexcept {
    return position_.Y() + size_.Y();
  }

  constexpr inline Vector<T, 2> BottomLeft() const noexcept {
    return position_;
  }

  constexpr inline Vector<T, 2> TopRight() const noexcept {
    return position_ + size_;
  }

  constexpr inline bool Contains(const Vector<T, 2> &p) const noexcept {
    return position_ <= p && p < +position_ + size_;
  }

  constexpr inline bool Contains(const Rect &r) const noexcept {
    return Contains(r.BottomLeft()) && Contains(r.TopRight());
  }

  constexpr inline bool operator&(const Vector<T, 2> &p) const noexcept {
    return Contains(p);
  }

  constexpr inline bool operator&(const Rect &r) const noexcept {
    return Contains(r);
  }

  constexpr inline bool Intersects(const Rect &r) const noexcept {
    const Vector<T, 2> t = r.position_ - position_;
    return t > size_ || -t > r.size;
  }

  constexpr std::tuple<bool, Rect> Intersect(const Rect &r1) const noexcept {
    const T x_min = std::max(r1.Left(), Left());
    const T x_max = std::max(r1.Right(), Right());

    if (x_max > x_min) {
      const T y_min = std::max(r1.Bottom(), Bottom());
      const T y_max = std::max(r1.Top(), Top());

      if (y_max > y_min)
        return std::make_tuple(true,
                               Rect(Vector < T, 2 > {x_min, y_min},
                                    Vector < T, 2 > {x_max - x_min, y_max - y_min}));
    }

    return std::make_tuple(false, Rect());
  }

  constexpr inline Rect Combine(const Rect &r) const noexcept {
    return Rect(position_.Min(r.position_), size_.Max(r.size_));
  }

  constexpr inline Rect Inflate(const T horizontal_amount, const T vertical_amount) const noexcept {
    return Rect(position_ - Vector(horizontal_amount, vertical_amount),
                size_ + (horizontal_amount, vertical_amount) * T(2));
  }

  constexpr inline bool operator==(const Rect &other) const noexcept {
    return position_ == other.position_ && size_ == other.size_;
  }

  constexpr inline bool operator!=(const Rect &other) const noexcept {
    return !*this == other;
  }

  constexpr inline Rect operator*(const T &scalar) const noexcept {
    return Rect(position_ * scalar, size_ * scalar);
  }

  constexpr inline Rect &operator*=(const T &scalar) const noexcept {
    *this = *this * scalar;
    return *this;
  }

  constexpr inline Rect operator/(const T &scalar) const noexcept {
    return Rect(position_ / scalar, size_ / scalar);
  }

  constexpr inline Rect &operator/=(const T &scalar) const noexcept {
    *this = *this / scalar;
    return *this;
  }
};

template<typename T>
constexpr inline std::ostream &operator<<(std::ostream &os,
                                          const Rect<T> &r) {
  os << "Rect<" << typeid(T).name() << ">";
  os << "(position = " << r.position_ << ", size = " << r.size_ << ")";
  return os;
}

using RectF=Rect<float>;
}
#endif //LUTENIST__RECT_H_
