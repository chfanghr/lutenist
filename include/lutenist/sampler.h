//
// Created by 方泓睿 on 2020/3/29.
//

#ifndef LUTENIST__SAMPLER_H_
#define LUTENIST__SAMPLER_H_

namespace lutenist {
class Sampler {
 public:
  enum class AddressMode { kClamp, kRepeat, kMirror };

  enum class Filter { kPoint, kLinear };

  AddressMode address_mode_x_ = AddressMode::kClamp;
  AddressMode address_mode_y_ = AddressMode::kClamp;

  Filter filter_ = Filter::kPoint;
};
}
#endif // LUTENIST__SAMPLER_H_
