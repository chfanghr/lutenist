//
// Created by 方泓睿 on 2020/3/30.
//

#ifndef LUTENIST_EXAMPLES_DEMO_TEXTURE_SHADER_H_
#define LUTENIST_EXAMPLES_DEMO_TEXTURE_SHADER_H_

#include <lutenist/shader.h>

namespace demo {

class TextureShader : public lutenist::Shader {
 public:
  V2F VertexShader(const lutenist::Matrix4F &model_view_projection, const lutenist::Vertex &vertex) override;

  lutenist::Color FragmentShader(const V2F &input,
                                 std::array<std::shared_ptr<lutenist::Sampler>, 2> &samplers,
                                 std::array<std::shared_ptr<lutenist::Texture>, 2> &textures) override;
};
}
#endif //LUTENIST_EXAMPLES_DEMO_TEXTURE_SHADER_H_
