//
// Created by 方泓睿 on 2020/3/30.
//

#ifndef LUTENIST__VERTEX_H_
#define LUTENIST__VERTEX_H_

#include "vector.h"
#include "color.h"

namespace lutenist {
class Vertex {
 public:

  constexpr inline Vertex() noexcept = default;

  Vertex(const Vector4F &position, const Color &color,
         const Vector2F &tex_coord, const Vector3F &normal) :
      position_(position), color_(color), tex_coord_{tex_coord}, normal_(normal) {}

  Vector4F position_{};
  Color color_{};
  std::array<Vector2F, 2> tex_coord_{};
  Vector3F normal_{};
};
}
#endif //LUTENIST__VERTEX_H_
