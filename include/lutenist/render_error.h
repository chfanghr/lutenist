//
// Created by 方泓睿 on 2020/3/30.
//

#ifndef LUTENIST__RENDER_ERROR_H_
#define LUTENIST__RENDER_ERROR_H_

#include <stdexcept>
#include <string>

namespace lutenist {
// TODO classify errors
class RenderError : public std::runtime_error {
 public:
  explicit RenderError(const std::string &msg) : std::runtime_error(msg) {}
  explicit RenderError(const char *msg) : std::runtime_error(msg) {}
};
}
#endif //LUTENIST__RENDER_ERROR_H_
