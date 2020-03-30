//
// Created by 方泓睿 on 2020/3/30.
//

#ifndef LUTENIST_EXAMPLES_DEMO_UTILS_H_
#define LUTENIST_EXAMPLES_DEMO_UTILS_H_

#include <ostream>
#include <cstdlib>

namespace demo {
namespace color {
class Modifier {
 public:
  enum class Code : int32_t {
    FG_RED = 31,
    FG_GREEN = 32,
    FG_BLUE = 34,
    FG_DEFAULT = 39,
    BG_RED = 41,
    BG_GREEN = 42,
    BG_BLUE = 44,
    BG_DEFAULT = 49
  };
 private:
  Code code_;
 public:
  explicit Modifier(Code code) : code_(code) {}

  friend std::ostream &operator<<(std::ostream &os, const Modifier &mod) {
    return os << "\033[" << (int32_t) mod.code_ << "m";
  }
};
}

void ShowFPS(double fps, bool reset = false);
}
#endif //LUTENIST_EXAMPLES_DEMO_UTILS_H_
