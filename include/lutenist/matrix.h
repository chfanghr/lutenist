//
// Created by 方泓睿 on 2020/3/26.
//

#ifndef LUTENIST__MATRIX_H_
#define LUTENIST__MATRIX_H_

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdlib>
#include <ostream>
#include <type_traits>
#include <typeinfo>

#include "convex_volume.h"
#include "plane.h"
#include "quaternion.h"
#include "vector.h"

namespace lutenist {
enum class RotationAxis { kX, kY, kZ };

template<typename T, size_t C, size_t R,
    typename std::enable_if_t<(C > 0 && R > 0)> * = nullptr>
class Matrix {
 public:
  std::array<T, C * R> data_{};

  constexpr inline Matrix() noexcept = default;

  template<typename... A>
  explicit constexpr inline Matrix(const A &... args) noexcept
      : data_{static_cast<T>(args)...} {}

  constexpr inline T &operator[](size_t index) noexcept {
    assert(index < R * C);
    return data_[index];
  }

  constexpr inline T operator[](size_t index) const {
    assert(index < R * C);
    return data_[index];
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == Y)> * = nullptr>
  static constexpr inline Matrix Identity() noexcept {
    Matrix res{0};
    res.SetIdentity();

    return res;
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == Y)> * = nullptr>
  constexpr inline void SetIdentity() noexcept {
    data_ = {0};

    for (size_t i = 0; i < X; i++)
      data_[i * X + i] = 1;
  }

  constexpr inline Matrix operator+(const T &scalar) const noexcept {
    Matrix res(*this);
    std::transform(std::begin(res.data_), std::end(res.data_),
                   std::begin(res.data_),
                   [&](const T &val) { return val + scalar; });
    return res;
  }

  constexpr inline Matrix &operator+=(const T &scalar) noexcept {
    std::transform(std::begin(data_), std::end(data_), std::begin(data_),
                   [&](const T &a) { return a + scalar; });
    return *this;
  }

  constexpr inline Matrix operator+(const Matrix &m) const noexcept {
    Matrix res;
    std::transform(std::begin(data_), std::end(data_), std::begin(m.data_),
                   std::begin(res.data_),
                   [](const T &a, const T &b) { return a + b; });
    return res;
  }

  constexpr inline Matrix &operator+=(const Matrix &m) noexcept {
    std::transform(std::begin(data_), std::end(data_), std::begin(m.data_),
                   std::begin(data_),
                   [](const T &a, const T &b) { return a + b; });
    return *this;
  }

  constexpr inline Matrix operator-(const T &scalar) const noexcept {
    Matrix res(*this);
    std::transform(std::begin(res.data_), std::end(res.data_),
                   std::begin(res.data_),
                   [&](const T &val) { return val - scalar; });
    return res;
  }

  constexpr inline Matrix &operator-=(const T &scalar) noexcept {
    std::transform(std::begin(data_), std::end(data_), std::begin(data_),
                   [&](const T &a) { return a - scalar; });
    return *this;
  }

  constexpr inline Matrix operator-(const Matrix &m) const noexcept {
    Matrix res;
    std::transform(std::begin(data_), std::end(data_), std::begin(m.data_),
                   std::begin(res.data_),
                   [](const T &a, const T &b) { return a - b; });
    return res;
  }

  constexpr inline Matrix &operator-=(const Matrix &m) noexcept {
    std::transform(std::begin(data_), std::end(data_), std::begin(m.data_),
                   std::begin(data_),
                   [](const T &a, const T &b) { return a - b; });
    return *this;
  }

  constexpr inline Matrix operator*(const T scalar) const noexcept {
    Matrix res{};
    std::transform(std::begin(data_), std::end(data_), std::begin(res.data_),
                   [&](const T &a) { return a * scalar; });
    return res;
  }

  constexpr inline Matrix &operator*=(const T scalar) noexcept {
    std::transform(std::begin(data_), std::end(data_), std::begin(data_),
                   [&](const T &a) { return a * scalar; });
    return *this;
  }

  constexpr inline Matrix operator/(const T scalar) const noexcept {
    Matrix res{};
    std::transform(std::begin(data_), std::end(data_), std::begin(res.data_),
                   [&](const T &a) { return a / scalar; });
    return res;
  }

  constexpr inline Matrix &operator/=(const T scalar) noexcept {
    std::transform(std::begin(data_), std::end(data_), std::begin(data_),
                   [&](const T &a) { return a / scalar; });
    return *this;
  }

  constexpr inline bool operator==(const Matrix &m) const noexcept {
    for (size_t i = 0; i < C * R; i++)
      if (data_[i] != m[i])
        return false;

    return true;
  }

  constexpr inline bool operator!=(const Matrix &m) const noexcept {
    return !*this == m;
  }

  constexpr inline Vector<T, C> GetRow(const size_t row) const noexcept {
    assert(row < R);
    Vector<T, C> res{};
    std::copy(std::begin(data_) + row * C, std::begin(data_) + row * C + C,
              std::begin(res.data_));
    return res;
  }

  constexpr inline Vector<T, R> GetCol(const size_t col) const noexcept {
    assert(col < C);
    Vector<T, R> res{};
    for (size_t r = 0; r < R; r++)
      res[r] = data_[r * C + col];
    return res;
  }

  constexpr inline void SetRow(const size_t row,
                               const Vector<T, C> &vec) noexcept {
    assert(row < R);
    std::copy(std::begin(vec.data_), std::end(vec.data_),
              std::begin(data_) + row * C);
  }

  constexpr inline void SetCol(const size_t col,
                               const Vector<T, R> &vec) noexcept {
    assert(col < C);
    for (size_t r = 0; r < R; r++)
      data_[r * C + col] = vec[r];
  }

  constexpr inline T &At(const size_t col, const size_t row) noexcept {
    assert(col < C);
    assert(row < R);

    return data_[row * C + col];
  }

  constexpr inline T At(const size_t col, const size_t row) const noexcept {
    assert(col < C);
    assert(row < R);

    return data_[row * C + col];
  }

  constexpr inline T &operator()(const size_t col, const size_t row) noexcept {
    return At(col, row);
  }

  constexpr inline T operator()(const size_t col, const size_t row) const
  noexcept {
    return At(col, row);
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 4 && Y == 4)> * = nullptr>
  constexpr inline void SetLookAt(const Vector<T, 3> &eye_position,
                                  const Vector<T, 3> &target_position,
                                  const Vector<T, 3> &up_ori) noexcept {
    auto up = up_ori.Normalize();

    auto z_axis = (target_position - eye_position).Normalize();
    auto x_axis = up.Cross(z_axis).Normalize();
    auto y_axis = z_axis.Cross(x_axis).Normalize();

    for (size_t r = 0; r < 3; r++)
      SetRow(r, Vector<T, C>{x_axis[r], y_axis[r], z_axis[r], T(0)});

    SetRow(3, Vector<T, C>{x_axis.Dot(-eye_position), y_axis.Dot(-eye_position),
                           z_axis.Dot(-eye_position), T(1)});
  }

  constexpr inline void Clear() noexcept { data_ = {0}; }

  [[nodiscard]] constexpr inline size_t GetIndex(size_t col, size_t row) const
  noexcept {
    assert(col < C);
    assert(row < R);
    return row * C + col;
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == Y)> * = nullptr>
  constexpr inline Matrix operator*(const Matrix &m) const noexcept {
    Matrix res{};

    for (size_t r = 0; r < R; r++)
      for (size_t c = 0; c < C; c++)
        res[GetIndex(c, r)] = GetRow(r).Dot(m.GetCol(c));

    return res;
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == Y)> * = nullptr>
  constexpr inline Matrix &operator*=(const Matrix &m) const noexcept {
    *this = *this * m;
    return *this;
  }

  constexpr inline Vector<T, R> operator*(const Vector<T, R> &vec) const
  noexcept {
    Vector<T, R> res{0};
    for (size_t c = 0; c < C; c++)
      res += GetCol(c) * vec[c];
    return res;
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 4 && Y == 4)> * = nullptr>
  constexpr inline void
  SetPerspective(const T &field_of_view, const T &aspect_ratio,
                 const T &z_near_plane, const T &z_far_plane) noexcept {
    assert(z_far_plane != z_near_plane);

    const T theta = field_of_view / T(2);

    if (std::fabs(std::fmod(theta, T(M_PI) / T(2))) <=
        std::numeric_limits<T>::min())
      return;

    const T divisor = std::tan(theta);
    assert(divisor);

    const T factor = T(1) / divisor;

    Clear();

    data_[0] = (T(1) / aspect_ratio) * factor;
    data_[5] = factor;
    data_[10] = z_far_plane / (z_far_plane - z_near_plane);
    data_[11] = T(1);
    data_[14] = -z_near_plane * z_far_plane / (z_far_plane - z_near_plane);
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 4 && Y == 4)> * = nullptr>
  constexpr inline void SetOrthographicFromSize(const T &width, const T &height,
                                                const T &z_near_plane,
                                                const T &z_far_plane) noexcept {
    const T half_width = width / T(2);
    const T half_height = height / T(2);

    SetOrthographicOffCenter(-half_width, half_width, -half_height, half_height,
                             z_near_plane, z_far_plane);
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 4 && Y == 4)> * = nullptr>
  constexpr inline void
  SetOrthographicOffCenter(const T &right, const T &left, const T &bottom,
                           const T &top, const T &z_near_plane,
                           const T &z_far_plane) noexcept {
    assert(right != left);
    assert(top != bottom);
    assert(z_far_plane != z_near_plane);

    Clear();

    data_[0] = T(2) / (right - left);
    data_[5] = T(2) / (top - bottom);
    data_[10] = T(1) / (z_far_plane - z_near_plane);
    data_[12] = (left + right) / (left - right);
    data_[13] = (bottom + top) / (bottom - top);
    data_[14] = z_near_plane / (z_near_plane - z_far_plane);
    data_[15] = T(1);
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 3 && Y == 3)> * = nullptr>
  constexpr inline void SetScale(const Vector<T, 2> &scale) noexcept {
    data_ = Identity().data_;

    data_[0] = scale.X();
    data_[4] = scale.Y();
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 3 && Y == 3)> * = nullptr>
  constexpr inline void SetScale(const Vector<T, 3> &scale) noexcept {
    SetIdentity();

    data_[0] = scale.X();
    data_[5] = scale.Y();
    data_[10] = scale.Z();
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 3 && Y == 3)> * = nullptr>
  constexpr inline void SetRotation(const T &angle) noexcept {
    SetIdentity();

    const T c = std::cos(angle);
    const T s = std::sin(angle);

    data_[0] = c;
    data_[3] = -s;
    data_[1] = s;
    data_[4] = c;
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 4 && Y == 4)> * = nullptr>
  constexpr inline void SetRotation(const Vector<T, 3> &axis,
                                    const T &angle) noexcept {
    const T x = axis.X();
    const T y = axis.Y();
    const T z = axis.Z();

    const T squared_length = axis.LengthSquared();

    if (T(1) == squared_length ||
        (T(1) - std::numeric_limits<T>::min() <= squared_length &&
            squared_length <= T(1) + std::numeric_limits<T>::min())) {
      const T length = axis.Length();
      if (length > std::numeric_limits<T>::min()) {
        x /= length;
        y /= length;
        z /= length;
      }
    }

    const T c = std::cos(angle);
    const T s = std::sin(angle);

    const T t = T(1) - c;
    const T tx = t * x;
    const T ty = t * y;
    const T tz = t * z;
    const T txy = tx * y;
    const T txz = tx * z;
    const T tyz = ty * z;
    const T sx = s * x;
    const T sy = s * y;
    const T sz = s * z;

    SetCol(0, Vector<T, R>{c + tx * x, txy - sz, txz + sy, T(0)});
    SetCol(1, Vector<T, R>{txy + sz, c + ty * y, tyz - sz, T(0)});
    SetCol(2, Vector<T, R>{txz - sy, tyz + sx, c + tz * z, T(0)});
    SetCol(3, Vector<T, R>{0, 0, 0, 1});
  }

  template<RotationAxis A, size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 4 && Y == 4)> * = nullptr>
  constexpr inline void SetRotationAxis(const T &angle) noexcept {
    SetIdentity();

    const T c = std::cos(angle);
    const T s = std::sin(angle);

    switch (A) {
      case RotationAxis::kX:data_[5] = c;
        data_[9] = -s;
        data_[6] = s;
        data_[10] = c;
        return;
      case RotationAxis::kY:data_[0] = c;
        data_[8] = s;
        data_[2] = -s;
        data_[10] = c;
        return;
      case RotationAxis::kZ:data_[0] = c;
        data_[4] = -s;
        data_[1] = s;
        data_[5] = c;
        return;
    }
    assert(false);
  }

  template<size_t N1, size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 3 && Y == 3)> * = nullptr,
      typename std::enable_if_t<(N1 == 3 || N1 == 2)> * = nullptr>
  constexpr inline void
  SetTranslation(const Vector<T, N1> &translation) noexcept {
    SetIdentity();

    data_[6] = translation.X();
    data_[7] = translation.Y();
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 4 && Y == 4)> * = nullptr>
  constexpr inline void
  SetTranslation(const Vector<T, 3> &translation) noexcept {
    SetIdentity();

    data_[12] = translation.X();
    data_[13] = translation.Y();
    data_[14] = translation.Z();
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 1 && Y == 1)> * = nullptr>
  constexpr inline T Determinant() const noexcept {
    return data_[0];
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 2 && Y == 2)> * = nullptr>
  constexpr inline T Determinant() const noexcept {
    return data_[0] * data_[3] - data_[1] * data_[2];
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 3 && Y == 3)> * = nullptr>
  constexpr inline T Determinant() const noexcept {
    const T a1 = data_[GetIndex(0, 0)] *
        Matrix<T, 2, 2>{data_[GetIndex(1, 1)], data_[GetIndex(2, 1)],
                        data_[GetIndex(1, 2)], data_[GetIndex(2, 2)]}
            .Determinant();
    const T a2 = -data_[GetIndex(1, 0)] *
        Matrix<T, 2, 2>{data_[GetIndex(0, 1)], data_[GetIndex(2, 1)],
                        data_[GetIndex(0, 2)], data_[GetIndex(2, 2)]}
            .Determinant();
    const T a3 = data_[GetIndex(2, 0)] *
        Matrix<T, 2, 2>{data_[GetIndex(0, 1)], data_[GetIndex(1, 1)],
                        data_[GetIndex(0, 2)], data_[GetIndex(1, 2)]}
            .Determinant();

    return a1 + a2 + a3;
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 4 && Y == 4)> * = nullptr>
  constexpr inline T Determinant() const noexcept {
    const T a1 = data_[GetIndex(0, 0)] *
        Matrix<T, 3, 3>{data_[GetIndex(1, 1)], data_[GetIndex(2, 1)],
                        data_[GetIndex(3, 1)], data_[GetIndex(1, 2)],
                        data_[GetIndex(2, 2)], data_[GetIndex(3, 2)],
                        data_[GetIndex(1, 3)], data_[GetIndex(2, 3)],
                        data_[GetIndex(3, 3)]}
            .Determinant();
    const T a2 = -data_[GetIndex(1, 0)] *
        Matrix<T, 3, 3>{data_[GetIndex(0, 1)], data_[GetIndex(2, 1)],
                        data_[GetIndex(3, 1)], data_[GetIndex(0, 2)],
                        data_[GetIndex(2, 2)], data_[GetIndex(3, 2)],
                        data_[GetIndex(0, 3)], data_[GetIndex(2, 3)],
                        data_[GetIndex(3, 3)]}
            .Determinant();
    const T a3 = data_[GetIndex(2, 0)] *
        Matrix<T, 3, 3>{data_[GetIndex(0, 1)], data_[GetIndex(1, 1)],
                        data_[GetIndex(3, 1)], data_[GetIndex(0, 2)],
                        data_[GetIndex(1, 2)], data_[GetIndex(3, 2)],
                        data_[GetIndex(0, 3)], data_[GetIndex(1, 3)],
                        data_[GetIndex(3, 3)]}
            .Determinant();
    const T a4 = -data_[GetIndex(3, 0)] *
        Matrix<T, 3, 3>{data_[GetIndex(0, 1)], data_[GetIndex(1, 1)],
                        data_[GetIndex(2, 1)], data_[GetIndex(0, 2)],
                        data_[GetIndex(1, 2)], data_[GetIndex(2, 2)],
                        data_[GetIndex(0, 3)], data_[GetIndex(1, 3)],
                        data_[GetIndex(2, 3)]}
            .Determinant();

    return a1 + a2 + a3 + a4;
  }

  template<VectorPosition V, size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 4 && Y == 4)> * = nullptr>
  constexpr inline Vector<T, 3> GetVector() const noexcept {
    auto make_vector = [&](size_t x_idx, size_t y_idx, size_t z_idx,
                           bool all_neg = false) -> Vector<T, 3> {
      if (!all_neg)
        return Vector<T, 3>{data_[x_idx], data_[y_idx], data_[z_idx]};
      return Vector<T, 3>{-data_[x_idx], -data_[y_idx], -data_[z_idx]};
    };

    switch (V) {
      case VectorPosition::kUp:return make_vector(4, 5, 6);
      case VectorPosition::kDown:return make_vector(4, 5, 6, true);
      case VectorPosition::kLeft:return make_vector(0, 1, 2, true);
      case VectorPosition::kRight:return make_vector(0, 1, 2);
      case VectorPosition::kForward:return make_vector(8, 9, 10, true);
      case VectorPosition::kBack:return make_vector(8, 9, 10);
    }
  }

  constexpr inline Matrix<T, R, C> Transpose() const noexcept {
    Matrix<T, R, C> res{};
    for (size_t c = 0; c < C; c++)
      res.SetRow(c, GetCol(c));
    return res;
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 4 && Y == 4)> * = nullptr>
  constexpr inline Matrix Invert() const noexcept {
    return InverseTranspose().Transpose();
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 4 && Y == 4)> * = nullptr>
  constexpr inline Matrix InverseTranspose() const noexcept {
    const auto adj = Adjoint();
    const auto det = Determinant();
    const auto inv_det = T(1) / det;

    return adj * inv_det;
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 4 && Y == 4)> * = nullptr>
  constexpr inline T Minor(size_t c, size_t r) const noexcept {
    Matrix<T, 3, 3> cut_down{};

    for (size_t i = 0; i < 3; i++)
      for (size_t j = 0; j < 3; j++) {
        auto row = i < r ? i : i + 1;
        auto col = j < c ? j : j + 1;

        cut_down[cut_down.GetIndex(j, i)] = data_[GetIndex(col, row)];
      }

    return cut_down.Determinant();
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 4 && Y == 4)> * = nullptr>
  constexpr inline T Cofactor(size_t c, size_t r) const noexcept {
    const T sign = (r + c) % 2 == 0 ? 1 : -1;
    return sign * Minor(c, r);
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 4 && Y == 4)> * = nullptr>
  constexpr inline Matrix Adjoint() const noexcept {
    Matrix res{};
    for (size_t c = 0; c < C; c++)
      for (size_t r = 0; r < R; r++)
        res[GetIndex(c, r)] = Cofactor(c, r);
    return res;
  }

  template<size_t N1>
  constexpr inline Vector<T, R> TransformVector(const Vector<T, N1> &vec) const
  noexcept {
    return *this * Vector<T, R>(vec);
  }

  template<size_t N1>
  constexpr inline Vector<T, R> TransformPoint(const Vector<T, 3> &point) {
    return TransformVector(point);
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 3 && Y == 3)> * = nullptr>
  constexpr inline Vector<T, 3> GetTranslation() const noexcept {
    return Vector<T, 3>{data_[6], data_[7]};
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 4 && Y == 4)> * = nullptr>
  constexpr inline Vector<T, 4> GetTranslation() const noexcept {
    return Vector<T, 4>{data_[12], data_[13, data_[14]]};
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 3 && Y == 3)> * = nullptr>
  constexpr inline Vector<T, 2> GetScale() const noexcept {
    return Vector<T, 2>{Vector<T, 2>{data_[0], data_[1]}.Length(),
                        Vector<T, 2>{data_[3], data_[4]}.Length()};
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 4 && Y == 4)> * = nullptr>
  constexpr inline Vector<T, 3> GetScale() const noexcept {
    return Vector<T, 3>{
        Vector<T, 3>{data_[0], data_[1], data_[2]}.Length(),
        Vector<T, 3>{data_[4], data_[5], data_[6]}.Length(),
        Vector<T, 3>{data_[8], data_[9], data_[10]}.Length(),
    };
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 4 && Y == 4)> * = nullptr>
  constexpr inline void SetRotation(const Quaternion<T> &qua) noexcept {
    const T xx = qua.X() * qua.X();
    const T xy = qua.X() * qua.Y();
    const T xz = qua.X() * qua.Z();
    const T xw = qua.X() * qua.W();
    const T yy = qua.Y() * qua.Y();
    const T yz = qua.Y() * qua.Z();
    const T yw = qua.Y() * qua.W();
    const T zz = qua.Z() * qua.Z();
    const T zw = qua.Z() * qua.W();

    *this = Matrix{
        1 - 2 * (yy + zz),
        2 * (xy - zw),
        2 * (xz + yw),
        0,
        2 * (xy + zw),
        1 - 2 * (xx + zz),
        2 * (yz - xw),
        0,
        2 * (xz - yw),
        2 * (yz + xw),
        1 - 2 * (xx + yy),
        0,
        0,
        0,
        0,
        1,
    };
  }

  template<size_t X = C, size_t Y = R,
      typename std::enable_if_t<(X == 4 && Y == 4)> * = nullptr>
  constexpr inline Quaternion<T> GetRotation() noexcept {
    const Vector<T, 3> scale = GetScale();
    const Matrix<T, 3, 3> m{
        data_[0] / scale.X(), data_[1] / scale.X(), data_[2] / scale.X(),
        data_[3] / scale.Y(), data_[4] / scale.Y(), data_[5] / scale.Y(),
        data_[6] / scale.Z(), data_[7] / scale.Z(), data_[8] / scale.Z(),
    };

    return Quaternion<T>(Vector<T, 4>{
        std::copysign(
            std::sqrt(std::max(static_cast<T>(0), T(1) + m[m.GetIndex(0, 0)] -
                m[m.GetIndex(1, 1)] -
                m[m.GetIndex(2, 2)])) /
                T(2),
            m[m.GetIndex(1, 2)] - m[m.GetIndex(2, 1)]),
        std::copysign(
            std::sqrt(std::max(static_cast<T>(0), T(1) - m[m.GetIndex(0, 0)] +
                m[m.GetIndex(1, 1)] -
                m[m.GetIndex(2, 2)])) /
                T(2),
            m[m.GetIndex(2, 0)] - m[m.GetIndex(0, 2)]),
        std::copysign(
            std::sqrt(std::max(static_cast<T>(0), T(1) - m[m.GetIndex(0, 0)] -
                m[m.GetIndex(1, 1)] +
                m[m.GetIndex(2, 2)])) /
                T(2),
            m[m.GetIndex(0, 1)] - m[m.GetIndex(1, 0)]),
        std::sqrt(std::max(static_cast<T>(0), T(1) + m[m.GetIndex(0, 0)] +
            m[m.GetIndex(1, 1)] +
            m[m.GetIndex(2, 2)])) /
            T(2),
    }
                             .Normalize());
  }

  constexpr inline ConvexVolume<T> GetFrustum() const noexcept {
    return ConvexVolume<T>{
        GetFrustumPlane<FrustumPlanePosition::kLeft>(),
        GetFrustumPlane<FrustumPlanePosition::kRight>(),
        GetFrustumPlane<FrustumPlanePosition::kBottom>(),
        GetFrustumPlane<FrustumPlanePosition::kTop>(),
        GetFrustumPlane<FrustumPlanePosition::kNear>(),
        GetFrustumPlane<FrustumPlanePosition::kFar>(),
    };
  }

  template<FrustumPlanePosition F, std::size_t X = C, std::size_t Y = R,
      typename std::enable_if<(X == 4 && Y == 4)>::type * = nullptr>
  constexpr inline Plane<T> GetFrustumPlane() const noexcept {
    auto helper = [&](std::array<size_t, 8> indexes, bool is_plus = true) {
      if (is_plus)
        return Plane<T>::MakeFrustumPlane(data_[indexes[0]] + data_[indexes[1]],
                                          data_[indexes[2]] + data_[indexes[3]],
                                          data_[indexes[4]] + data_[indexes[5]],
                                          data_[indexes[6]] +
                                              data_[indexes[8]]);
      return Plane<T>::MakeFrustumPlane(data_[indexes[0]] - data_[indexes[1]],
                                        data_[indexes[2]] - data_[indexes[3]],
                                        data_[indexes[4]] - data_[indexes[5]],
                                        data_[indexes[6]] - data_[indexes[8]]);
    };
    switch (F) {
      case FrustumPlanePosition::kLeft:return helper({3, 0, 7, 4, 11, 8, 15, 12});
      case FrustumPlanePosition::kRight:return helper({3, 0, 7, 4, 11, 8, 15, 12}, false);
      case FrustumPlanePosition::kBottom:return helper({3, 1, 7, 5, 11, 9, 15, 13});
      case FrustumPlanePosition::kTop:return helper({3, 1, 7, 5, 11, 9, 15, 13}, false);
      case FrustumPlanePosition::kNear:return helper({3, 2, 7, 6, 11, 10, 15, 14});
      case FrustumPlanePosition::kFar:return helper({3, 2, 7, 6, 11, 10, 15, 14}, false);
      default:assert(false);
    }
  }
};

template<typename T, size_t C, size_t R>
constexpr inline std::ostream &operator<<(std::ostream &os,
                                          const Matrix<T, C, R> &m) noexcept {
  os << "Matrix<" << typeid(T).name() << ", " << C << ", " << R << ">(";
  for (size_t r = 0; r < R; r++) {
    os << "(";
    bool is_first = true;
    for (size_t c = 0; c < C; c++) {
      if (!is_first)
        os << ", ";
      os << m.data_[r * C + c];
      is_first = false;
    }
    os << ");";
  }
  os << ")";
  return os;
}

template<typename T, size_t N>
constexpr inline Vector<T, N> &operator*=(Vector<T, N> &vec,
                                          const Matrix<T, 4, 4> &m) noexcept {
  vec = Vector<T, N>(m.TransformVector(vec));
  return vec;
}

using Matrix4F = Matrix<float, 4, 4>;
using Matrix3F = Matrix<float, 3, 3>;
}
#endif // LUTENIST__MATRIX_H_
