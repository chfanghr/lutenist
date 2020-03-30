//
// Created by 方泓睿 on 2020/3/30.
//

#include <string>
#include <csignal>
#include <mutex>
#include <iostream>

#include <SDL.h>

#include <lutenist.h>
#include <lutenist/demo/app.h>
#include <lutenist/demo/utils.h>

class App : public demo::App {
 public:
  explicit App(const std::string &app_name) : demo::App() {
    signal(SIGINT, SignalHandler);
    demo::App::size_ = lutenist::Size2(1024, 768);
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
      throw std::runtime_error("failed to init sdl");
    sdl_window_ = SDL_CreateWindow(app_name.c_str(),
                                   SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, // NOLINT(hicpp-signed-bitwise)
                                   this->size_.Width(), this->size_.Height(), SDL_WINDOW_SHOWN);
    if (!sdl_window_)
      throw std::runtime_error("failed to create window");

    sdl_renderer_ = SDL_CreateRenderer(sdl_window_, -1, 0);
    if (!sdl_renderer_)
      throw std::runtime_error("failed to create renderer");

    SetupTexture();

//    SDL_AddEventWatch(ResizingEventWatcher, this);

    demo::App::render_target_ = std::make_shared<lutenist::RenderTarget>(size_);

    SDLDraw();

    demo::App::Setup();
  }

  ~App() {
    SDL_DestroyTexture(sdl_texture_);
    SDL_DestroyRenderer(sdl_renderer_);
    SDL_DestroyWindow(sdl_window_);
  }

  double Draw() {
    auto frame_start = std::chrono::system_clock::now();
    demo::App::Render();
    auto frame_end = std::chrono::system_clock::now();

    std::chrono::duration<double> diff = frame_end - frame_start;

    return diff.count();
  }

  int Run() {
    double local_frame_time_sum = 0;
    size_t local_frame_count = 0;

    while (running_) {
      mu_.lock();
      auto frame_time = Draw();
      mu_.unlock();

      ++frame_count_;
      ++local_frame_count;
      local_frame_time_sum += frame_time / 100;
      avg_frame_time_ = (avg_frame_time_ + frame_time) / (avg_frame_time_ == 0 ? 1 : 2);

      if (local_frame_count == 100) {
        demo::ShowFPS(1.0 / local_frame_time_sum);
        local_frame_time_sum = local_frame_count = 0;
      }

      SDL_Event event{};
      SDL_PollEvent(&event);
      if (event.type == SDL_QUIT)
        running_ = false;

      SDLDraw();
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
  void SDLDraw() {
    SDL_UpdateTexture(sdl_texture_,
                      nullptr,
                      this->render_target_->frame_buffer_.GetData().data(),
                      int(this->size_.Width() * 4));

    SDL_RenderCopy(sdl_renderer_, sdl_texture_, nullptr, nullptr);
    SDL_RenderPresent(sdl_renderer_);
  }

  void SetupTexture() {
    sdl_texture_ = SDL_CreateTexture(sdl_renderer_, SDL_PIXELFORMAT_RGBA8888,
                                     SDL_TEXTUREACCESS_STREAMING, this->size_.Width(), this->size_.Height());
    if (!sdl_texture_)
      throw std::runtime_error("failed to create sdl renderer or texture");
  }

  static int ResizingEventWatcher(void *data, SDL_Event *event) {
    App *obj = (App *) data;
    if (event->type == SDL_WINDOWEVENT &&
        event->window.event == SDL_WINDOWEVENT_RESIZED) {
      SDL_Window *win = SDL_GetWindowFromID(event->window.windowID);
      if (win == obj->sdl_window_) {
        SDL_DisplayMode dm;
        if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
          throw std::runtime_error("failed to get resolution");
        obj->mu_.lock();
        obj->size_ = lutenist::Size2(dm.w, dm.h);
        SDL_DestroyTexture(obj->sdl_texture_);
        SDL_DestroyRenderer(obj->sdl_renderer_);
        obj->SetupTexture();
        obj->OnResized();
        obj->mu_.unlock();
      }
    }

    return 0;
  }

  static void SignalHandler(int) {
    running_ = false;
  }

  uint64_t frame_count_ = 0;
  double avg_frame_time_ = 0;

  static bool running_;

  SDL_Window *sdl_window_;
  SDL_Renderer *sdl_renderer_{};
  SDL_Texture *sdl_texture_{};

  std::mutex mu_;
};

bool App::running_ = true;

int main() {
  return App("demo").Run();
}