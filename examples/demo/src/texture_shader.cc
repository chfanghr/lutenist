//
// Created by 方泓睿 on 2020/3/30.
//

#include <lutenist/demo/texture_shader.h>

namespace demo {

lutenist::Shader::V2F TextureShader::VertexShader(const lutenist::Matrix4F &model_view_projection,
                                                  const lutenist::Vertex &vertex) {
  V2F res{};

  res.position = model_view_projection * vertex.position_;
  res.color = vertex.color_;
  res.tex_coords = vertex.tex_coord_;
  res.normal = vertex.normal_;

  return res;
}

lutenist::Color TextureShader::FragmentShader(const lutenist::Shader::V2F &input,
                                              std::array<std::shared_ptr<lutenist::Sampler>, 2> &samplers,
                                              std::array<std::shared_ptr<lutenist::Texture>, 2> &textures) {
  const auto sample_color = samplers[0] ? textures[0]->Sample(*samplers[0], input.tex_coords[0]) : lutenist::Color();

  lutenist::Color res{};

  for (size_t i = 0; i < 4; i++)
    res[i] = input.color[i] * sample_color[i];

  return res;
}
}