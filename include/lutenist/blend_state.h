//
// Created by 方泓睿 on 2020/3/30.
//

#ifndef LUTENIST__BLEND_STATE_H_
#define LUTENIST__BLEND_STATE_H_

#include "color.h"
#include "utils.h"

namespace lutenist {
class BlendState {
 public:
  enum class Factor {
    kZero,
    kOne,
    kSrcColor,
    kInvSrcColor,
    kSrcAlpha,
    kInvSrcAlpha,
    kDstAlpha,
    kInvDstAlpha,
    kDstColor,
    kInvDstColor,
    kSrcAlphaSat,
    kBlendFactor,
    kInvBlendFactor,
  };

  enum class Operation {
    kAdd,
    kSubtract,
    kRevSubtract,
    kMin,
    kMax
  };

  bool enabled_ = false;
  Color blend_factor_{};

  Factor color_blend_src_ = Factor::kOne;
  Factor color_blend_dst_ = Factor::kZero;
  Factor alpha_blend_src_ = Factor::kOne;
  Factor alpha_blend_dst_ = Factor::kZero;

  Operation color_op_ = Operation::kAdd;
  Operation alpha_op_ = Operation::kAdd;
};

constexpr inline float GetValue(const BlendState::Factor factor,
                                const float src_color = 0,
                                const float src_alpha = 0,
                                const float dst_color = 0,
                                const float dst_alpha = 0,
                                const float blend_factor = 0) {
  switch (factor) {
    case BlendState::Factor::kZero:return 0.0F;
    case BlendState::Factor::kOne:return 1.0F;
    case BlendState::Factor::kSrcColor:return src_color;
    case BlendState::Factor::kInvSrcColor:return 1.0F - src_color;
    case BlendState::Factor::kSrcAlpha:return src_alpha;
    case BlendState::Factor::kInvSrcAlpha:return 1 - src_alpha;
    case BlendState::Factor::kDstAlpha:return dst_alpha;
    case BlendState::Factor::kInvDstAlpha:return 1 - dst_alpha;
    case BlendState::Factor::kSrcAlphaSat:return std::min(src_alpha, 1 - dst_alpha);
    case BlendState::Factor::kBlendFactor:return blend_factor;
    case BlendState::Factor::kInvBlendFactor:return 1 - blend_factor;
    case BlendState::Factor::kDstColor:return dst_color;
    case BlendState::Factor::kInvDstColor:return 1 - dst_color;
  }
}

constexpr inline float GetValue(BlendState::Operation operation, float a = 0, float b = 0) {
  switch (operation) {
    case BlendState::Operation::kAdd:return Clamp(a + b, 0.0F, 1.0F);
    case BlendState::Operation::kSubtract:return Clamp(a - b, 0.0F, 1.0F);
    case BlendState::Operation::kRevSubtract:return Clamp(b - a, 0.0F, 1.0F);
    case BlendState::Operation::kMin:return std::min(a, b);
    case BlendState::Operation::kMax:return std::max(a, b);
  }
}

constexpr inline Color GetValue(const BlendState &blend_state, const Color &src_color, const Color &dst_color) {
  return Color(GetValue(blend_state.color_op_,
                        src_color.R() * GetValue(blend_state.color_blend_src_,
                                                 src_color.R(),
                                                 src_color.A(),
                                                 dst_color.R(),
                                                 dst_color.A(),
                                                 blend_state.blend_factor_.R()),
                        dst_color.R() * GetValue(blend_state.color_blend_dst_,
                                                 src_color.R(),
                                                 src_color.A(),
                                                 dst_color.R(),
                                                 dst_color.A(),
                                                 blend_state.blend_factor_.R())),
               GetValue(blend_state.color_op_,
                        src_color.R() * GetValue(blend_state.color_blend_src_,
                                                 src_color.G(),
                                                 src_color.A(),
                                                 dst_color.G(),
                                                 dst_color.A(),
                                                 blend_state.blend_factor_.G()),
                        dst_color.R() * GetValue(blend_state.color_blend_dst_,
                                                 src_color.G(),
                                                 src_color.A(),
                                                 dst_color.G(),
                                                 dst_color.A(),
                                                 blend_state.blend_factor_.G())),
               GetValue(blend_state.color_op_,
                        src_color.B() * GetValue(blend_state.color_blend_src_,
                                                 src_color.B(),
                                                 src_color.A(),
                                                 dst_color.B(),
                                                 dst_color.A(),
                                                 blend_state.blend_factor_.G()),
                        dst_color.B() * GetValue(blend_state.color_blend_dst_,
                                                 src_color.B(),
                                                 src_color.A(),
                                                 dst_color.B(),
                                                 dst_color.A(),
                                                 blend_state.blend_factor_.G())),
               GetValue(blend_state.alpha_op_,
                        src_color.A() * GetValue(blend_state.color_blend_src_,
                                                 src_color.A(),
                                                 src_color.A(),
                                                 dst_color.A(),
                                                 dst_color.A(),
                                                 blend_state.blend_factor_.A()),
                        dst_color.A() * GetValue(blend_state.color_blend_dst_,
                                                 src_color.A(),
                                                 src_color.A(),
                                                 dst_color.A(),
                                                 dst_color.A(),
                                                 blend_state.blend_factor_.A())));
}
}

#endif //LUTENIST__BLEND_STATE_H_
