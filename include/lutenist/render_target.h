//
// Created by 方泓睿 on 2020/3/29.
//

#ifndef LUTENIST__RENDER_TARGET_H_
#define LUTENIST__RENDER_TARGET_H_

#include "texture.h"

namespace lutenist {
class RenderTarget {
 public:
  Texture frame_buffer_, depth_buffer_;

  explicit RenderTarget(const Size2 &size = Size2(0, 0)) :
      frame_buffer_(Texture::PixelFormat::kRGBA8, size),
      depth_buffer_(Texture::PixelFormat::kFloat32, size) {}
};
}
#endif //LUTENIST__RENDER_TARGET_H_
