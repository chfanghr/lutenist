//
// Created by 方泓睿 on 2020/3/30.
//

#ifndef LUTENIST__SHADER_H_
#define LUTENIST__SHADER_H_

#include "vector.h"
#include "color.h"
#include "vertex.h"
#include "matrix.h"
#include "sampler.h"
#include "texture.h"

namespace lutenist {
class Shader {
 public:
  struct V2F {
    Vector4F position;
    Color color;
    std::array<Vector2F, 2> tex_coords;
    Vector3F normal;
  };

  virtual ~Shader() noexcept = default;

  virtual V2F VertexShader(const Matrix4F &model_view_projection, const Vertex &vertex) = 0;

  virtual Color FragmentShader(const V2F &input,
                               std::array<std::shared_ptr<Sampler>, 2> &samplers,
                               std::array<std::shared_ptr<Texture>, 2> &textures) = 0;
};
}
#endif //LUTENIST__SHADER_H_
