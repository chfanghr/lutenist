//
// Created by 方泓睿 on 2020/3/28.
//

#ifndef LUTENIST__PLANE_H_
#define LUTENIST__PLANE_H_

#include "vector.h"

namespace lutenist {
template<typename T>
class Plane : public Vector<T, 4> {
 public:
  constexpr inline Plane() noexcept = default;

  template<typename T1, size_t N>
  constexpr inline explicit Plane(const Vector<T1, N> &vec) noexcept
      : Vector<T, 4>(vec) {}

  template<typename... A>
  constexpr inline explicit Plane(const A &... args) : Vector<T, 4>(args...) {}

  constexpr inline T &A() noexcept { return this->X(); }
  constexpr inline T A() const noexcept { return this->X(); }

  constexpr inline T &B() noexcept { return this->Y(); }
  constexpr inline T B() const noexcept { return this->Y(); }

  constexpr inline T &C() noexcept { return this->Z(); }
  constexpr inline T C() const noexcept { return this->Z(); }

  constexpr inline T &D() noexcept { return this->W(); }
  constexpr inline T D() const noexcept { return this->W(); }

  constexpr inline Plane Flip() const noexcept { return Plane(-*this); }

  constexpr inline static Plane
  MakeFrustumPlane(const T &a, const T &b, const T &c, const T &d) noexcept {
    return Plane(Vector<T, 4>(a, b, c, d).Normalize());
  }
};

template<typename T>
constexpr inline std::ostream &operator<<(std::ostream &os,
                                          const Plane<T> &vec) {
  os << "Plane<" << typeid(T).name() << ">(";

  bool is_first = true;

  for (const auto &val : vec.data_) {
    if (!is_first)
      os << ", ";
    os << val;
    is_first = false;
  }

  os << ")";
  return os;
}

using PlaneF = Plane<float>;

enum class FrustumPlanePosition { kRight, kLeft, kBottom, kTop, kNear, kFar };
}

#endif // LUTENIST__PLANE_H_
