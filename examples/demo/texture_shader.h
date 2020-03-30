//
// Created by 方泓睿 on 2020/3/30.
//

#ifndef LUTENIST_EXAMPLES_DEMO_TEXTURE_SHADER_H_
#define LUTENIST_EXAMPLES_DEMO_TEXTURE_SHADER_H_

#include <lutenist/shader.h>

namespace demo {

class TextureShader : public lutenist::Shader {
 public:
  V2F VertexShader(const lutenist::Matrix4F &model_view_projection, const lutenist::Vertex &vertex) override {
    V2F res{};

    res.position = model_view_projection * vertex.position_;
    res.color = vertex.color_;
    res.tex_coords = vertex.tex_coord_;
    res.normal = vertex.normal_;

    return res;
  }

  lutenist::Color FragmentShader(const V2F &input,
                                 std::array<std::shared_ptr<lutenist::Sampler>, 2> &samplers,
                                 std::array<std::shared_ptr<lutenist::Texture>, 2> &textures) override {
    const auto sample_color = samplers[0] ? textures[0]->Sample(*samplers[0], input.tex_coords[0]) : lutenist::Color();

    lutenist::Color res{};

    for (size_t i = 0; i < 4; i++)
      res[i] = input.color[i] * sample_color[i];

    return res;
  }
};
}
#endif //LUTENIST_EXAMPLES_DEMO_TEXTURE_SHADER_H_
