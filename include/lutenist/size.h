//
// Created by 方泓睿 on 2020/3/28.
//

#ifndef LUTENIST__SIZE_H_
#define LUTENIST__SIZE_H_

#include "vector.h"

namespace lutenist {
template<typename T, size_t N>
class Size : public Vector<T, N> {
 public:
  constexpr inline Size() noexcept = default;

  template<typename... A>
  constexpr inline explicit Size(const A &... args) : Vector<T, N>(args...) {}

  template<typename T1, size_t N1>
  constexpr inline explicit Size(const Vector<T1, N1> &vec) noexcept
      : Vector<T, N>(vec) {}

  template<typename T1, size_t N1, size_t X = N,
      typename std::enable_if_t<(X != N1)> * = nullptr>
  constexpr inline explicit Size(const Size<T1, N1> &s) noexcept {
    for (size_t i = 0; i < N && i < N1; i++)
      this->data_[i] = s.data_[i];
  }

  template<std::size_t X = N,
      typename std::enable_if<(X >= 1)>::type * = nullptr>
  constexpr inline T &Width() noexcept {
    return this->X();
  }

  template<std::size_t X = N,
      typename std::enable_if<(X >= 1)>::type * = nullptr>
  constexpr inline T Width() const noexcept {
    return this->X();
  }

  template<std::size_t X = N,
      typename std::enable_if<(X >= 2)>::type * = nullptr>
  constexpr inline T &Height() noexcept {
    return this->Y();
  }

  template<std::size_t X = N,
      typename std::enable_if<(X >= 2)>::type * = nullptr>
  constexpr inline T Height() const noexcept {
    return this->Y();
  }

  template<std::size_t X = N,
      typename std::enable_if<(X >= 3)>::type * = nullptr>
  constexpr inline T &Depth() noexcept {
    return this->Z();
  }

  template<std::size_t X = N,
      typename std::enable_if<(X >= 3)>::type * = nullptr>
  constexpr inline T Depth() const noexcept {
    return this->Z();
  }

  constexpr inline void SetScale(const Vector<T, N> &scale) noexcept {
    for (size_t i = 0; i < N; i++)
      this->data_[i] *= scale[i];
  }

  constexpr inline Size operator+(const Size &s) const noexcept {
    return Size((*(const Vector<T, N> *) this) + (*(const Vector<T, N> *) &s));
  }

  constexpr inline Size &operator+=(const Size &s) noexcept {
    *this = *this + s;
    return *this;
  }

  constexpr inline Size operator-(const Size &s) const noexcept {
    return Size((*(const Vector<T, N> *) this) - (*(const Vector<T, N> *) &s));
  }

  constexpr inline Size &operator-=(const Size &s) noexcept {
    *this = *this - s;
    return *this;
  }

  constexpr inline Size operator*(const T scalar) const noexcept {
    return Size((*(const Vector<T, N> *) this) * scalar);
  }

  constexpr inline Size operator*(const Vector<T, N> &vec) const noexcept {
    auto res = *this;
    for (size_t i = 0; i < N; i++)
      res[i] *= vec[i];
    return res;
  }

  constexpr inline Size operator/(const Vector<T, N> &vec) const noexcept {
    auto res = *this;
    for (size_t i = 0; i < N; i++)
      res[i] /= vec[i];
    return res;
  }

  constexpr inline Size &operator*=(const T scalar) noexcept {
    *this = Size((*(const Vector<T, N> *) this) * scalar);
    return *this;
  }

  constexpr inline Size operator/(const T scalar) const noexcept {
    return Size((*(const Vector<T, N> *) this) / scalar);
  }

  constexpr inline Size &operator/=(const T scalar) noexcept {
    *this = Size((*(const Vector<T, N> *) this) / scalar);
    return *this;
  }

  constexpr inline T Volume() const noexcept {
    T res(1);
    for (const auto &v : this->data_)
      res *= v;
    return res;
  }
};

template<typename T, size_t N>
constexpr inline std::ostream &operator<<(std::ostream &os,
                                          const Size<T, N> &vec) {
  os << "Size<" << typeid(T).name() << ", " << N << ">(";

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

using Size2 = Size<size_t, 2>;
using Size3 = Size<size_t, 3>;
using Size2F = Size<float, 2>;
using Size3F = Size<float, 3>;
}

#endif // LUTENIST__SIZE_H_
