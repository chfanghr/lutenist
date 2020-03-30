//
// Created by 方泓睿 on 2020/3/30.
//

#include <chrono>
#include <iostream>
#include <csignal>

#include <lutenist/demo/app.h>
#include <lutenist/demo/utils.h>

class App final : public demo::App {
 public:
  App() : demo::App() {
    demo::App::size_ = lutenist::Size2(1024, 768);
    demo::App::render_target_ = std::make_shared<lutenist::RenderTarget>(size_);
    demo::App::Setup();

    signal(SIGINT, App::SignalHandler);
  }

  double Draw() {
    auto frame_start = std::chrono::system_clock::now();
    demo::App::Render();
    auto frame_end = std::chrono::system_clock::now();

    std::chrono::duration<double> diff = frame_end - frame_start;

    return diff.count();
  }

  void DoResize() { demo::App::OnResized(); }

  static bool running_;

  uint64_t frame_count_ = 0;
  double avg_frame_time_ = 0;

  int Run() {
    double local_frame_time_sum = 0;
    size_t local_frame_count = 0;

    while (running_) {
      auto frame_time = Draw();

      ++frame_count_;
      ++local_frame_count;
      local_frame_time_sum += frame_time / 100;
      avg_frame_time_ = (avg_frame_time_ + frame_time) / (avg_frame_time_ == 0 ? 1 : 2);

      if (local_frame_count == 100) {
        demo::ShowFPS(1.0 / local_frame_time_sum);
        local_frame_time_sum = local_frame_count = 0;
      }
    }

    std::cout << std::endl
              << demo::color::Modifier(demo::color::Modifier::Code::BG_BLUE)
              << demo::color::Modifier(demo::color::Modifier::Code::FG_RED)
              << "frame_count = " << frame_count_ << "; "
              << "avg_frame_time = " << avg_frame_time_ << "; "
              << "avg_fps = " << 1.0 / avg_frame_time_ << std::endl
              << demo::color::Modifier(demo::color::Modifier::Code::BG_DEFAULT)
              << demo::color::Modifier(demo::color::Modifier::Code::FG_DEFAULT);

    std::cout.flush();
    
    return EXIT_SUCCESS;
  }

 private:
  static void SignalHandler(int) { running_ = false; }
};

bool App::running_ = true;

int main() {
  return App().Run();
}