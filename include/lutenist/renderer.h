//
// Created by 方泓睿 on 2020/3/30.
//

#ifndef LUTENIST__RENDERER_H_
#define LUTENIST__RENDERER_H_

#include <memory>

#include "render_target.h"
#include "rect.h"
#include "shader.h"
#include "depth_state.h"
#include "blend_state.h"

namespace lutenist {
class Renderer {
 public:
  std::shared_ptr<RenderTarget> render_target_ = nullptr;

  std::shared_ptr<Shader> shader_ = nullptr;

  std::array<std::shared_ptr<Sampler>, 2> samplers_{nullptr, nullptr};
  std::array<std::shared_ptr<Texture>, 2> textures_{nullptr, nullptr};

  RectF view_port_{};
  RectF scissor_rect_ = RectF{0, 0, 1, 1};

  BlendState blend_state_;
  DepthState depth_state_;
 public:
  Renderer() = default;

  void Clear(Color color, float depth) noexcept(false);

 private:
  void DrawTriangles(const std::vector<uint32_t> &indices,
                     const std::vector<Vertex> &vertices,
                     const Matrix4F &model_view_projection) noexcept(false);

  void CheckRenderTarget() noexcept(false);

  void CheckShader() noexcept(false);

  struct RenderTargetData {
    uint32_t *frame_buffer_data;
    float *depth_buffer_data;
    size_t frame_buffer_size, depth_buffer_size;
  };

  RenderTargetData MakeRenderTargetData() noexcept(false);
};
}
#endif //LUTENIST__RENDERER_H_
