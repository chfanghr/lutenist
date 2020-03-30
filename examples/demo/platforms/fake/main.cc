//
// Created by 方泓睿 on 2020/3/30.
//

#include <chrono>
#include <iostream>
#include <csignal>

#include "../../app.h"

class App : public demo::App {
 public:
  App() : demo::App() {
    demo::App::size_ = lutenist::Size2(1024, 768);
    demo::App::render_target_ = std::make_shared<lutenist::RenderTarget>(size_);
    demo::App::Setup();

    signal(SIGINT, App::SignalHandler);
  }

  void Draw() {
    static size_t frame_counter = 0;

    auto frame_start = std::chrono::system_clock::now();
    demo::App::Render();
    auto frame_end = std::chrono::system_clock::now();

    std::chrono::duration<double> diff = frame_end - frame_start;

    std::cout << "frame " << frame_counter << " takes " << diff.count();
    std::cout << ", ";
    std::cout << "fps " << 1.0 / diff.count() << std::endl;

    ++frame_counter;
  }

  void DoResize() { demo::App::OnResized(); }

  static bool running_;

  void Run() {
    while (running_)
      Draw();
  }

 private:
  static void SignalHandler(int) { running_ = false; }
};

bool App::running_ = true;

int main() {
  App app;

  app.Run();

  return EXIT_SUCCESS;
}