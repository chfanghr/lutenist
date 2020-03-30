//
// Created by 方泓睿 on 2020/3/30.
//

#ifndef LUTENIST_EXAMPLES_DEMO_APP_H_
#define LUTENIST_EXAMPLES_DEMO_APP_H_

#include <string>

#include <lutenist.h>

namespace demo {
class App {
 public:
  App();
  virtual  ~App() = default;

  App(const App &) = delete;
  App(App &&) = delete;
  App &operator=(const App &) = delete;
  App &operator=(App &&) = delete;

  void Setup();

  void Render();

  static std::string GetResourcePath();
 protected:
  void OnResized();

  lutenist::Size2 size_;

  lutenist::Matrix4F projection_;
  lutenist::Matrix4F view_;
  lutenist::Matrix4F model_;

  float rotation_y_ = 0.0F;

  std::shared_ptr<lutenist::RenderTarget> render_target_;
  lutenist::Renderer renderer_;

  lutenist::BlendState blend_state_;
  lutenist::DepthState depth_state_;

  std::shared_ptr<lutenist::Shader> shader_;
  std::shared_ptr<lutenist::Sampler> sampler_;
  std::shared_ptr<lutenist::Texture> texture_;

  std::vector<size_t> indices;
  std::vector<lutenist::Vertex> vertices;
};
}

#endif //LUTENIST_EXAMPLES_DEMO_APP_H_
