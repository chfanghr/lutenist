//
// Created by 方泓睿 on 2020/3/30.
//

#include <iostream>
#include <iomanip>

#include <lutenist/demo/utils.h>

namespace demo {
void ShowFPS(double fps, bool reset) {
  static bool is_first_time = true;

  if (reset) is_first_time = true;

  std::cout << std::setprecision(6);

  std::cout << color::Modifier(color::Modifier::Code::BG_GREEN)
            << color::Modifier(color::Modifier::Code::FG_RED);

  if (!is_first_time)
    std::cout << "\b\b\b\b\b\b\b\b\b\b\b\b\b";
  else
    is_first_time = false;

  std::cout << "fps = " << fps;

  std::cout << color::Modifier(color::Modifier::Code::BG_DEFAULT)
            << color::Modifier(color::Modifier::Code::FG_DEFAULT);

  std::cout.flush();
}
}