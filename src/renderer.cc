//
// Created by 方泓睿 on 2020/3/30.
//

#include <lutenist/render_error.h>
#include <lutenist/renderer.h>

namespace lutenist {
void Renderer::Clear(const Color color, const float depth) {
  auto render_target_data = MakeRenderTargetData();

  const auto RGBA = color.GetIntValue();

  for (size_t p = 0; p < render_target_data.frame_buffer_size; p++)
    render_target_data.frame_buffer_data[p] = RGBA;

  for (size_t p = 0; p < render_target_data.depth_buffer_size; ++p)
    render_target_data.depth_buffer_data[p] = depth;
}

void Renderer::DrawTriangles(const std::vector<size_t> &indices,
                             const std::vector<Vertex> &vertices,
                             const lutenist::Matrix4F &model_view_projection) {
  auto render_target_data = MakeRenderTargetData();

  CheckShader();

  for (size_t i = 0; i + 2 < indices.size(); i += 3) {
    std::array<const Shader::V2F, 3> vs_outputs{
        shader_->VertexShader(model_view_projection, vertices[indices[i + 0]]),
        shader_->VertexShader(model_view_projection, vertices[indices[i + 1]]),
        shader_->VertexShader(model_view_projection, vertices[indices[i + 2]]),
    };

    std::array<Vector4F, 3> ndc_positions{
        vs_outputs[0].position,
        vs_outputs[1].position,
        vs_outputs[2].position,
    };

    // transform to normalized device coordinates
    for (auto &ndc_position:ndc_positions)
      ndc_position /= ndc_position[3];

    std::array<Vector2F, 3> viewport_positions{
        Vector2F(ndc_positions[0]),
        Vector2F(ndc_positions[1]),
        Vector2F(ndc_positions[2]),
    };

    Box2F bounding_box;

    for (auto viewport_position:viewport_positions) {
      // transform to viewport coordinates
      viewport_position.X() =
          viewport_position.X() * view_port_.size_.Width() / 2 + viewport_position.X() + view_port_.size_.Width() / 2;
      viewport_position.Y() =
          viewport_position.Y() * view_port_.size_.Height() / 2 + viewport_position.Y() + view_port_.size_.Height() / 2;
      if (viewport_position.X() < bounding_box.min_.X()) bounding_box.min_.X() = viewport_position.X();
      if (viewport_position.X() > bounding_box.max_.X()) bounding_box.max_.X() = viewport_position.X();
      if (viewport_position.Y() < bounding_box.min_.Y()) bounding_box.min_.Y() = viewport_position.Y();
      if (viewport_position.Y() > bounding_box.max_.Y()) bounding_box.max_.Y() = viewport_position.Y();
    }

    bounding_box.min_.X() = Clamp(bounding_box.min_.X(),
                                  0.0F,
                                  static_cast<float >(render_target_->frame_buffer_.GetWidth() - 1)
                                      * scissor_rect_.position_.X());
    bounding_box.max_.X() = Clamp(bounding_box.max_.X(),
                                  0.0F,
                                  static_cast<float >(render_target_->frame_buffer_.GetWidth() - 1)
                                      * scissor_rect_.position_.X());
    bounding_box.min_.Y() = Clamp(bounding_box.min_.Y(),
                                  0.0F,
                                  static_cast<float >(render_target_->frame_buffer_.GetHeight() - 1)
                                      * scissor_rect_.position_.Y());
    bounding_box.max_.Y() = Clamp(bounding_box.max_.Y(),
                                  0.0F,
                                  static_cast<float >(render_target_->frame_buffer_.GetHeight() - 1)
                                      * scissor_rect_.position_.Y());

    for (auto screen_y = static_cast<size_t>(bounding_box.min_.Y());
         screen_y <= static_cast<size_t>(bounding_box.max_.Y()); ++screen_y) {
      for (auto screen_x = static_cast<size_t>(bounding_box.min_.X());
           screen_x <= static_cast<size_t>(bounding_box.max_.X()); ++screen_x) {
        const auto s = Barycentric(viewport_positions[0],
                                   viewport_positions[1],
                                   viewport_positions[2],
                                   Vector2F{static_cast<float>(screen_x), static_cast<float>(screen_y)});

        if (s >= Vector3F{0, 0, 0}) {
          Vector3F clip{
              s.X() / vs_outputs[0].position.W(),
              s.Y() / vs_outputs[1].position.W(),
              s.Z() / vs_outputs[2].position.W(),
          };

          clip /= (clip.X() + clip.Y() + clip.Z());

          const float depth = ndc_positions[0].Z() * clip.X() +
              ndc_positions[1].Z() * clip.Y() +
              ndc_positions[1].Z() * clip.Z();

          size_t base_index = screen_y * render_target_->depth_buffer_.GetWidth() + screen_x;

          if (depth_state_.read_
              && render_target_data.depth_buffer_data[base_index] < depth)
            continue; // discard the pixel

          if (depth_state_.write_)
            render_target_data.depth_buffer_data[base_index] = depth;

          Shader::V2F ps_input;
          ps_input.position = Vector4F{clip.X(), clip.Y(), clip.Z(), 1.0F};
          ps_input.color =
              vs_outputs[0].color * clip.X() + vs_outputs[1].color * clip.Y() + vs_outputs[2].color * clip.Z();
          ps_input.tex_coords = {
              vs_outputs[0].tex_coords[0] * clip.X() +
                  vs_outputs[1].tex_coords[0] * clip.Y() +
                  vs_outputs[2].tex_coords[0] * clip.Z(),
              vs_outputs[0].tex_coords[1] * clip.X() +
                  vs_outputs[1].tex_coords[1] * clip.Y() +
                  vs_outputs[2].tex_coords[1] * clip.Z(),
          };

          ps_input.normal = vs_outputs[0].normal + vs_outputs[1].normal + vs_outputs[2].normal;

          const auto src_color = shader_->FragmentShader(ps_input, samplers_, textures_);

          if (blend_state_.enabled_) {
            auto *pixel = reinterpret_cast<uint8_t *>(&render_target_data.frame_buffer_data[base_index]);
            Color dst_color(pixel[0], pixel[1], pixel[2], pixel[3]);

            // alpha blend
            dst_color = GetValue(blend_state_, src_color, dst_color);
            render_target_data.frame_buffer_data[base_index] = dst_color.GetIntValue();
          } else
            render_target_data.frame_buffer_data[base_index] = src_color.GetIntValue();
        }
      }
    }
  }
}

void Renderer::CheckRenderTarget() {
  if (!render_target_)
    throw RenderError("no render target set");
}

void Renderer::CheckShader() {
  if (!shader_)
    throw RenderError("no shader set");
}

Renderer::RenderTargetData Renderer::MakeRenderTargetData() noexcept(false) {
  CheckRenderTarget();

  Renderer::RenderTargetData res{};

  res.frame_buffer_data = reinterpret_cast<uint32_t *>(render_target_->frame_buffer_.GetData().data());
  res.depth_buffer_data = reinterpret_cast<float *>(render_target_->depth_buffer_.GetData().data());

  const auto frame_buffer_size_tmp = render_target_->frame_buffer_.GetSize();
  res.frame_buffer_size = frame_buffer_size_tmp.Width() * frame_buffer_size_tmp.Height();

  const auto depth_buffer_size_tmp = render_target_->depth_buffer_.GetSize();
  res.depth_buffer_size = depth_buffer_size_tmp.Width() * depth_buffer_size_tmp.Height();

  return res;
}
}
