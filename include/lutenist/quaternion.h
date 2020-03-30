//
// Created by 方泓睿 on 2020/3/27.
//

#ifndef LUTENIST__QUATERNION_H_
#define LUTENIST__QUATERNION_H_

#include <tuple>

#include "vector.h"

namespace lutenist {
template<typename T>
class Quaternion : public Vector<T, 4> {
 public:
  template<size_t N>
  explicit constexpr inline Quaternion(const Vector<T, N> &vec) noexcept
      : Vector<T, 4>{0} {
    for (size_t i = 0; i < 4 && i < N; i++)
      Vector<T, 4>::data_[i] = vec[i];
  }

  template<typename... A>
  constexpr inline explicit Quaternion(const A &... args)
      : Vector<T, 4>{static_cast<T>(args)...} {}

  constexpr inline Quaternion() noexcept = default;

  constexpr inline Quaternion operator*(const Quaternion &q) const noexcept {
    const auto &v = this->data_;
    const auto &qv = q->data_;

    return Quaternion(
        v[0] * qv[3] + v[1] * qv[2] - v[2] * qv[1] + v[3] * qv[0],
        -v[0] * qv[2] + v[1] * qv[3] + v[2] * qv[0] + v[3] * qv[1],
        v[0] * qv[1] - v[1] * qv[0] + v[2] * qv[3] + v[3] * qv[2],
        -v[0] * qv[0] - v[1] * qv[1] - v[2] * qv[2] + v[3] * qv[3]);
  }

  constexpr inline Quaternion &operator*=(const Quaternion &q) const noexcept {
    *this = *this * q;
    return *this;
  }

  constexpr inline Quaternion Conjugate() const noexcept {
    return Quaternion{-this->data_[0], -this->data_[1], -this->data_[2],
                      this->data_[3]};
  }

  constexpr inline Quaternion Invert() const noexcept {
    const T squared = this->LengthSquared();
    if (squared <= std::numeric_limits<T>::min())
      return *this;

    return Quaternion{
        -this->data_[0] / squared,
        -this->data_[1] / squared,
        -this->data_[2] / squared,
        this->data_[3] / squared,
    };
  }

  constexpr inline T GetNorm() const noexcept {
    const T n = this->LengthSquared();

    if (n == T(1))
      return T(1);

    return this->Length();
  }

  constexpr inline void SetRotate(const T &angle,
                                  const Vector<T, 3> &axis) noexcept {
    const T c = std::cos(angle / T(2));
    const T s = std::sin(angle / T(2));

    const auto norm_axis = axis.Normalize();

    this->data_ = {norm_axis.X() * s, norm_axis.Y() * s, norm_axis.Z() * s, c};
  }

  constexpr inline std::tuple<T, Vector < T, 3>> GetRotation() const noexcept {
    T angle = T(2) * std::acos(this->Z());
    const T s = std::sqrt(T(1) - this->Z() * this->Z());
    if (s <= std::numeric_limits<T>::min())
      return std::make_tuple(angle, Vector<T, 3>(*this));
    return std::make_tuple(angle, Vector<T, 3>(*this) / s);
  }

  constexpr inline T GetEulerAngleX() const noexcept {
    return std::atan2(
        T(2) *
            (this->data_[1] * this->data_[2] + this->data_[3] * this->data_[0]),
        this->data_[3] * this->data_[3] - this->data_[0] * this->data_[0] -
            this->data_[1] * this->data_[1] + this->data_[2] * this->data_[2]);
  }

  constexpr inline T GetEulerAngleY() const noexcept {
    return std::asin(T(-2) * (this->data_[0] * this->data_[2] -
        this->data_[3] * this->data_[1]));
  }

  constexpr inline T GetEulerAngleZ() const noexcept {
    return std::atan2(
        T(2) *
            (this->data_[0] * this->data_[1] + this->data_[3] * this->data_[2]),
        this->data_[3] * this->data_[3] + this->data_[0] * this->data_[0] -
            this->data_[1] * this->data_[1] - this->data_[2] * this->data_[2]);
  }

  constexpr inline Vector<T, 3> GetEulerAngles() const noexcept {
    return Vector < T, 3 > {GetEulerAngleX(), GetEulerAngleY(), GetEulerAngleZ()};
  }

  constexpr inline void SetEulerAngles(const Vector<T, 3> &angles) noexcept {
    const T angle_r = angles.X();
    const T sr = std::sin(angle_r);
    const T cr = std::cos(angle_r);

    const T angle_p = angles.Y();
    const T sp = std::sin(angle_p);
    const T cp = std::cos(angle_p);

    const T angle_y = angles.Z();
    const T sy = std::sin(angle_y);
    const T cy = std::cos(angle_y);

    const T cpcy = cp * cy;
    const T spcy = sp * cy;
    const T cpsy = cp * sy;
    const T spsy = sp * sy;

    this->data_ = {sr * cpcy - cr * spsy, cr * spcy + sr * cpsy,
                   cr * cpsy - sr * spcy, cr * cpcy + sr * spsy};
  }

  constexpr inline Vector<T, 3> RotateVector(const Vector<T, 3> &vec) const
  noexcept {
    const Vector<T, 3> q{this->data_[0], this->data_[1], this->data_[2]};
    const Vector<T, 3> t = T(2) * q.Cross(vec);
    return vec + (this->data_[3] * t) + q.Cross(t);
  }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"
  constexpr inline Vector<T, 3> operator*(const Vector<T, 3> &vec) const
  noexcept {
    return RotateVector(vec);
  }
#pragma clang diagnostic pop

  template<VectorPosition V>
  constexpr inline Vector<T, 3> GetVector() const noexcept {
    Vector<T, 3> r{};

    switch (V) {
      case VectorPosition::kRight:r = Vector < T, 3 > {1, 0, 0};
        break;
      case VectorPosition::kUp:r = Vector < T, 3 > {0, 1, 0};
        break;
      case VectorPosition::kForward:r = Vector < T, 3 > {0, 0, 1};
        break;
      default:assert(false);
    }

    return RotateVector(r);
  }

  constexpr inline Quaternion Lerp(const Quaternion &q1, const Quaternion &q2,
                                   const T &t) const noexcept {
    return (q1 * (T(1) - t)) + (q2 * t);
  }
};

template<typename T>
constexpr inline std::ostream &operator<<(std::ostream &os,
                                          const Quaternion<T> &vec) {
  os << "Quaternion<" << typeid(T).name() << ">(";

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

using QuaternionF = Quaternion<float>;
}
#endif // LUTENIST__QUATERNION_H_
