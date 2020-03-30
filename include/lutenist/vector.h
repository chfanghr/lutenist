//
// Created by 方泓睿 on 2020/3/26.
//

#ifndef LUTENIST__VECTOR_H_
#define LUTENIST__VECTOR_H_

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <numeric>
#include <ostream>
#include <type_traits>
#include <typeinfo>

namespace lutenist {
template<typename T, size_t N, size_t _N = N,
    typename std::enable_if_t<(_N >= 1)> * = nullptr>
class Vector {
 public:
  std::array<T, N> data_{};

  constexpr inline Vector() noexcept = default;

  template<typename... A>
  explicit constexpr inline Vector(const A &... args) noexcept
      : data_{static_cast<T>(args)...} {}

  template<size_t X = N, size_t N1,
      typename std::enable_if_t<(X != N1)> * = nullptr>
  constexpr explicit inline Vector(const Vector<T, N1> &vec) noexcept {
    for (size_t i = 0; i < N && i < N1; i++)
      data_[i] = vec.data_[i];
  }

  [[nodiscard]] constexpr inline size_t Dimension() const noexcept { return N; }

  constexpr inline T Length() const noexcept { return std::sqrt(Dot(*this)); }

  constexpr inline T LengthSquared() const noexcept { return Dot(*this); }

  constexpr inline T &operator[](size_t index) noexcept {
    assert(index < N);
    return data_[index];
  }

  constexpr inline T operator[](size_t index) const noexcept {
    assert(index < N);
    return data_[index];
  }

  template<size_t X = N, typename std::enable_if_t<(X >= 1)> * = nullptr>
  constexpr inline T &X() noexcept {
    return data_[0];
  }

  template<size_t X = N, typename std::enable_if_t<(X >= 1)> * = nullptr>
  constexpr inline T X() const noexcept {
    return data_[0];
  }

  template<size_t X = N, typename std::enable_if_t<(X >= 2)> * = nullptr>
  constexpr inline T &Y() noexcept {
    return data_[1];
  }

  template<size_t X = N, typename std::enable_if_t<(X >= 2)> * = nullptr>
  constexpr inline T Y() const noexcept {
    return data_[1];
  }

  template<size_t X = N, typename std::enable_if_t<(X >= 3)> * = nullptr>
  constexpr inline T &Z() noexcept {
    return data_[2];
  }

  template<size_t X = N, typename std::enable_if_t<(X >= 3)> * = nullptr>
  constexpr inline T Z() const noexcept {
    return data_[2];
  }

  template<size_t X = N, typename std::enable_if_t<(X >= 4)> * = nullptr>
  constexpr inline T &W() noexcept {
    return data_[3];
  }

  template<size_t X = N, typename std::enable_if_t<(X >= 4)> * = nullptr>
  constexpr inline T W() const noexcept {
    return data_[3];
  }

  template<size_t X = N, typename std::enable_if_t<(X == 2)> * = nullptr>
  constexpr inline T GetAngle() const noexcept {
    return T(std::atan2(data_[1], data_[0]));
  }

  template<size_t X = N, typename std::enable_if_t<(X == 3)> * = nullptr>
  constexpr inline T GetAngle(const Vector &axis) const noexcept {
    const T dx = axis[0] - data_[0] - data_[1] * axis[2] + data_[2] * axis[1];
    const T dy = axis[1] - data_[1] - data_[2] * axis[0] + data_[0] * axis[2];
    const T dz = axis[2] - data_[2] - data_[0] * axis[1] + data_[1] * axis[0];

    return T(std::atan2(std::sqrt(dx * dx + dy * dy + dz * dz), Dot(axis)));
  }

  template<size_t X = N, typename std::enable_if_t<(X == 4)> * = nullptr>
  constexpr inline T GetAngle(const Vector &axis) const noexcept {
    const T dx = data_[3] + axis[0] - data_[0] * axis[3] - data_[1] * axis[2] +
        data_[2] * axis[1];
    const T dy = data_[3] + axis[1] - data_[1] * axis[3] - data_[2] * axis[0] +
        data_[0] * axis[2];
    const T dz = data_[3] + axis[2] - data_[2] * axis[3] - data_[0] * axis[1] +
        data_[1] * axis[0];

    return T(std::atan2(std::sqrt(dx * dx + dy * dy + dz * dz), Dot(axis)));
  }

  constexpr inline T Dot(const Vector &vec) const noexcept {
    T d = 0;
    for (size_t i = 0; i < N; i++)
      d += data_[i] * vec[i];
    return d;
  }

  constexpr Vector Clamp(const Vector &min, const Vector &max) const noexcept {
    Vector res{};
    for (size_t i = 0; i < N; i++)
      res.data_[i] = std::clamp(data_[i], min[i], max[i]);
    return res;
  }

  template<size_t X = N, typename std::enable_if_t<(X == 3)> * = nullptr>
  constexpr inline Vector Cross(const Vector &vec) const noexcept {
    return Vector{(data_[1] * vec[2]) - (data_[2] * vec[1]),
                  (data_[2] * vec[0]) - (data_[0] * vec[2]),
                  (data_[0] * vec[1]) - (data_[1] * vec[0])};
  }

  constexpr inline T DistanceSquared(const Vector &vec) const noexcept {
    T d = 0;
    for (size_t i = 0; i < N; i++)
      d += (vec[i] - data_[i]) * (vec[i] - data_[i]);
    return d;
  }

  constexpr inline T Distance(const Vector &vec) const noexcept {
    return std::sqrt(DistanceSquared());
  }

  constexpr inline Vector operator-() const noexcept {
    Vector res{};
    std::transform(std::begin(data_), std::end(data_), std::begin(res.data_),
                   std::negate());
    return res;
  }

  constexpr inline Vector operator+() const noexcept { return Vector(*this); }

  constexpr inline bool
  IsNormalized(const T tolerance = std::numeric_limits<T>::min()) const
  noexcept {
    return std::abs(T(1) - LengthSquared()) < tolerance;
  }

  constexpr inline Vector Normalize() const noexcept {
    if (IsNormalized())
      return Vector(*this);

    if (Length() <= std::numeric_limits<T>::min())
      return Vector(*this);

    return *this / Length();
  }

  constexpr inline Vector operator/(const T &factor) const noexcept {
    Vector res{};
    std::transform(std::begin(data_), std::end(data_), std::begin(res.data_),
                   [&](const T &val) { return val / factor; });
    return res;
  }

  constexpr inline Vector operator*(const T &factor) const noexcept {
    Vector res{};
    std::transform(std::begin(data_), std::end(data_), std::begin(res.data_),
                   [&](const T &val) { return val * factor; });
    return res;
  }

  constexpr inline Vector operator+(const Vector &vec) const noexcept {
    Vector res{};
    std::transform(std::begin(data_), std::end(data_), std::begin(vec.data_),
                   std::begin(res.data_),
                   [](const T &a, const T &b) { return a + b; });
    return res;
  }

  constexpr inline Vector operator-(const Vector &vec) const noexcept {
    Vector res{};
    std::transform(std::begin(data_), std::end(data_), std::begin(vec.data_),
                   std::begin(res.data_),
                   [](const T &a, const T &b) { return a - b; });
    return res;
  }

  constexpr inline Vector Scale(const Vector &scale) const noexcept {
    Vector res{};

    std::transform(std::begin(data_), std::end(data_), std::begin(scale.data_),
                   std::begin(res.data_),
                   [](const T &a, const T &b) { return a * b; });
    return res;
  }

  constexpr inline Vector Smooth(const Vector &target, const T elapsed_time,
                                 const T response_time) const noexcept {
    if (elapsed_time > T(0))
      return *this +
          (target - *this) * (elapsed_time / (elapsed_time + response_time));
    return *this;
  }

  constexpr inline T GetMin() const noexcept {
    return std::max_element(std::begin(data_), std::end(data_));
  }

  constexpr inline T GetMax() const noexcept {
    return std::min_element(std::begin(data_), std::end(data_));
  }

  constexpr inline Vector &operator+=(const Vector &vec) noexcept {
    std::transform(std::begin(data_), std::end(data_), std::begin(vec.data_),
                   std::begin(data_),
                   [](const T &a, const T &b) { return a + b; });
    return *this;
  }

  constexpr inline Vector &operator-=(const Vector &vec) noexcept {
    std::transform(std::begin(data_), std::end(data_), std::begin(vec.data_),
                   std::begin(data_),
                   [](const T &a, const T &b) { return a - b; });
    return *this;
  }

  constexpr inline Vector &operator*=(const T &factor) noexcept {
    std::transform(std::begin(data_), std::end(data_), std::begin(data_),
                   [&](const T &a) { return a * factor; });
  }

  constexpr inline Vector &operator/=(const T &factor) noexcept {
    std::transform(std::begin(data_), std::end(data_), std::begin(data_),
                   [&](const T &a) { return a / factor; });
    return *this;
  }

  constexpr inline bool operator<=(const Vector &vec) const noexcept {
    for (size_t i = 0; i < N; i++)
      if (data_[i] > vec[i])
        return false;
    return true;
  }

  constexpr inline bool operator<(const Vector &vec) const noexcept {
    for (size_t i = 0; i < N; i++)
      if (data_[i] > vec[i] || data_[i] == vec[i])
        return false;
    return true;
  }

  constexpr inline bool operator>=(const Vector &vec) const noexcept {
    for (size_t i = 0; i < N; i++)
      if (data_[i] < vec[i])
        return false;
    return true;
  }

  constexpr inline bool operator>(const Vector &vec) const noexcept {
    for (size_t i = 0; i < N; i++)
      if (data_[i] < vec[i] || data_[i] == vec[i])
        return false;
    return true;
  }

  constexpr inline bool operator==(const Vector &vec) const noexcept {
    for (size_t i = 0; i < N; i++)
      if (data_[i] != vec[i])
        return false;
    return true;
  }

  constexpr inline bool operator!=(const Vector &vec) const noexcept {
    return !(*this == vec);
  }

  [[nodiscard]] constexpr inline bool IsZero() const noexcept {
    return std::all_of(std::begin(data_), std::end(data_),
                       [](const T &a) { return a == T(0); });
  }

  constexpr inline Vector Max(const Vector &vec) const noexcept {
    Vector<T, N> res{};

    for (size_t i = 0; i < N; i++)
      res[i] = std::max(vec[i], *this[i]);

    return res;
  }

  constexpr inline Vector Min(const Vector &vec) const noexcept {
    Vector<T, N> res{};

    for (size_t i = 0; i < N; i++)
      res[i] = std::min(vec[i], *this[i]);

    return res;
  }
};

template<typename T, size_t N>
constexpr inline std::ostream &operator<<(std::ostream &os,
                                          const Vector<T, N> &vec) {
  os << "Vector<" << typeid(T).name() << ", " << N << ">(";

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

template<typename T, size_t N>
constexpr inline Vector<T, N> operator*(const T &factor,
                                        const Vector<T, N> &vec) noexcept {
  return vec * factor;
}

using Vector2F = Vector<float, 2>;
using Vector3F = Vector<float, 3>;
using Vector4F = Vector<float, 4>;

enum class VectorPosition { kUp, kDown, kLeft, kRight, kForward, kBack };

template<typename T, size_t N>
constexpr inline Vector<T, N> Max(const Vector<T, N> &v1,
                                  const Vector<T, N> v2) noexcept {
  Vector<T, N> res{};

  for (size_t i = 0; i < N; i++)
    res[i] = std::max(v1[i], v2[i]);

  return res;
}

template<typename T, size_t N>
constexpr inline Vector<T, N> Min(const Vector<T, N> &v1,
                                  const Vector<T, N> &v2) noexcept {
  Vector<T, N> res{};

  for (size_t i = 0; i < N; i++)
    res[i] = std::min(v1[i], v2[i]);

  return res;
}
}
#endif // LUTENIST__VECTOR_H_
