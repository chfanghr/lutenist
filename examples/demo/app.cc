//
// Created by 方泓睿 on 2020/3/30.
//

#include "bmp.h"
#include "app.h"
#include "texture_shader.h"

namespace demo {
App::App() :
    indices{
        0, 1, 2, 1, 3, 2, // front
        4, 5, 6, 5, 7, 6, // back
        16, 17, 18, 17, 19, 18, // bottom
        20, 21, 22, 21, 23, 22, // top
        8, 9, 10, 9, 11, 10, // left
        12, 13, 14, 13, 15, 14 // right
    },
    vertices{
        // front
        lutenist::Vertex(lutenist::Vector4F(-20.0F, -20.0F, -20.0F, 1.0F),
                         lutenist::Color(0xFF0000FF),
                         lutenist::Vector2F(0.0F, 0.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),
        lutenist::Vertex(lutenist::Vector4F(-20.0F, 20.0F, -20.0F, 1.0F),
                         lutenist::Color(0x00FF00FF),
                         lutenist::Vector2F(0.0F, 1.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),
        lutenist::Vertex(lutenist::Vector4F(20.0F, -20.0F, -20.0F, 1.0F),
                         lutenist::Color(0x0000FFFF),
                         lutenist::Vector2F(1.0F, 0.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),
        lutenist::Vertex(lutenist::Vector4F(20.0F, 20.0F, -20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFFF),
                         lutenist::Vector2F(1.0F, 1.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),

        // back
        lutenist::Vertex(lutenist::Vector4F(-20.0F, -20.0F, 20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFFF),
                         lutenist::Vector2F(0.0F, 0.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),
        lutenist::Vertex(lutenist::Vector4F(-20.0F, 20.0F, 20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFFF),
                         lutenist::Vector2F(0.0F, 1.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),
        lutenist::Vertex(lutenist::Vector4F(20.0F, -20.0F, 20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFFF),
                         lutenist::Vector2F(1.0F, 0.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),
        lutenist::Vertex(lutenist::Vector4F(20.0F, 20.0F, 20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFFF),
                         lutenist::Vector2F(1.0F, 1.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),

        // left
        lutenist::Vertex(lutenist::Vector4F(-20.0F, -20.0F, -20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFFF),
                         lutenist::Vector2F(0.0F, 0.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),
        lutenist::Vertex(lutenist::Vector4F(-20.0F, 20.0F, -20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFFF),
                         lutenist::Vector2F(0.0F, 4.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),
        lutenist::Vertex(lutenist::Vector4F(-20.0F, -20.0F, 20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFFF),
                         lutenist::Vector2F(4.0F, 0.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),
        lutenist::Vertex(lutenist::Vector4F(-20.0F, 20.0F, 20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFFF),
                         lutenist::Vector2F(4.0F, 4.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),

        // right
        lutenist::Vertex(lutenist::Vector4F(20.0F, -20.0F, -20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFA0),
                         lutenist::Vector2F(0.0F, 0.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),
        lutenist::Vertex(lutenist::Vector4F(20.0F, 20.0F, -20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFA0),
                         lutenist::Vector2F(0.0F, 1.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),
        lutenist::Vertex(lutenist::Vector4F(20.0F, -20.0F, 20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFA0),
                         lutenist::Vector2F(1.0F, 0.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),
        lutenist::Vertex(lutenist::Vector4F(20.0F, 20.0F, 20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFA0),
                         lutenist::Vector2F(1.0F, 1.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),

        // bottom
        lutenist::Vertex(lutenist::Vector4F(-20.0F, -20.0F, -20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFFF),
                         lutenist::Vector2F(0.0F, 0.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),
        lutenist::Vertex(lutenist::Vector4F(-20.0F, -20.0F, 20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFFF),
                         lutenist::Vector2F(0.0F, 1.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),
        lutenist::Vertex(lutenist::Vector4F(20.0F, -20.0F, -20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFFF),
                         lutenist::Vector2F(1.0F, 0.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),
        lutenist::Vertex(lutenist::Vector4F(20.0F, -20.0F, 20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFFF),
                         lutenist::Vector2F(1.0F, 1.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),

        // top
        lutenist::Vertex(lutenist::Vector4F(-20.0F, 20.0F, -20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFFF),
                         lutenist::Vector2F(0.0F, 0.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),
        lutenist::Vertex(lutenist::Vector4F(-20.0F, 20.0F, 20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFFF),
                         lutenist::Vector2F(0.0F, 1.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),
        lutenist::Vertex(lutenist::Vector4F(20.0F, 20.0F, -20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFFF),
                         lutenist::Vector2F(1.0F, 0.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F)),
        lutenist::Vertex(lutenist::Vector4F(20.0F, 20.0F, 20.0F, 1.0F),
                         lutenist::Color(0xFFFFFFFF),
                         lutenist::Vector2F(1.0F, 1.0F),
                         lutenist::Vector3F(0.0F, 0.0F, 1.0F))
    }, sampler_(std::make_shared<lutenist::Sampler>()), shader_(std::make_shared<TextureShader>()) {
  projection_.SetIdentity();
  view_.SetIdentity();
  model_.SetIdentity();

  BMP cube(GetResourcePath() + "/cube.bmp");
  texture_ =
      std::make_shared<lutenist::Texture>(lutenist::Texture::PixelFormat::kRGBA8, cube.GetWidth(), cube.GetHeight());
  texture_->SetLevel(0, cube.GetData());
  texture_->GenerateMipMaps();

  sampler_->address_mode_x_ = lutenist::Sampler::AddressMode::kRepeat;
  sampler_->address_mode_y_ = lutenist::Sampler::AddressMode::kRepeat;
  sampler_->filter_ = lutenist::Sampler::Filter::kLinear;

  blend_state_.color_blend_src_ = lutenist::BlendState::Factor::kSrcAlpha;
  blend_state_.color_blend_dst_ = lutenist::BlendState::Factor::kInvSrcAlpha;
  blend_state_.color_op_ = lutenist::BlendState::Operation::kAdd;
  blend_state_.alpha_blend_src_ = lutenist::BlendState::Factor::kOne;
  blend_state_.alpha_blend_dst_ = lutenist::BlendState::Factor::kOne;
  blend_state_.alpha_op_ = lutenist::BlendState::Operation::kAdd;

  depth_state_.read_ = true;
  depth_state_.write_ = true;
}

void App::Setup() {
  render_target_ = std::make_shared<lutenist::RenderTarget>(size_);

  projection_.SetPerspective(lutenist::kTau / 6,
                             static_cast<float >(size_.Width()) / static_cast<float>(size_.Height()),
                             1.0F,
                             1000.0F);

  view_.SetTranslation(lutenist::Vector<float, 3>{0, 0, 100});
}

void App::Render() {
  renderer_.render_target_ = render_target_;

  rotation_y_ += 0.05F;
  model_.SetRotationAxis<lutenist::RotationAxis::kY>(rotation_y_);

  auto model_view_projection = projection_ * view_ * model_;

  renderer_.view_port_ = lutenist::RectF(0, 0, static_cast<float>(size_.Width()), static_cast<float>(size_.Height()));
  renderer_.blend_state_ = blend_state_;
  renderer_.depth_state_ = depth_state_;
  renderer_.Clear(lutenist::Color(uint8_t(255), 255, 255, 255), 1000.0F);

  renderer_.shader_ = shader_;
  renderer_.textures_[0] = texture_;
  renderer_.samplers_[0] = sampler_;

  renderer_.DrawTriangles(indices, vertices, model_view_projection);
}

void App::OnResized() {
  render_target_ = std::make_shared<lutenist::RenderTarget>(size_);

  projection_.SetPerspective(lutenist::kTau / 6,
                             static_cast<float >(size_.Width()) / static_cast<float>(size_.Height()),
                             1.0F,
                             1000.0F);
}

std::string App::GetResourcePath() { return "resources"; }
}