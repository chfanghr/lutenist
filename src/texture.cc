//
// Created by 方泓睿 on 2020/3/29.
//

#include <lutenist/texture.h>

namespace lutenist {
Texture::Texture(lutenist::Texture::PixelFormat pixel_format, size_t width,
                 size_t height, bool mip_maps)
    : Texture(pixel_format, Size2(width, height), mip_maps) {}

Texture::Texture(Texture::PixelFormat pixel_format, const Size2 &size,
                 bool mip_maps)
    : pixel_format_(pixel_format), mip_maps_(mip_maps) {
  Resize(size);
}

void Texture::Resize(const Size2 &size) {
  size_ = size;

  const auto width = size.Width();
  const auto height = size.Height();

  levels_.clear();

  const auto pixel_size = GetPixelSize(pixel_format_);

  if (pixel_size > 0 && width > 0 && height > 0) {
    levels_.emplace_back(width * height * pixel_size);

    if (mip_maps_) {
      auto new_width = width;
      auto new_height = height;

      while (new_width > 1 || new_height > 1) {
        new_width >>= 1U;
        new_height >>= 1U;

        if (new_width < 1)
          new_width = 1;
        if (new_height < 1)
          new_height = 1;

        levels_.emplace_back(new_width * new_height * pixel_size);
      }
    }
  }
}

void Texture::SetLevel(size_t level, const std::vector<uint8_t> &buffer) {
  const auto pixel_size = GetPixelSize(pixel_format_);

  if (buffer.size() != size_.Width() * size_.Height() * pixel_size)
    throw std::runtime_error("invalid buffer size");

  if (level >= levels_.size())
    levels_.resize(level + 1);

  levels_[level] = buffer;
}

Color Texture::GetPixel(size_t level, const Vector<size_t, 2> &pos) const {
  const auto &buffer = levels_[level];
  const size_t base_index = pos.Y() * size_.Width() + pos.X();

  switch (pixel_format_) {
    case PixelFormat::kR8: {
      const auto *r = &buffer[base_index * 1];
      return Color(*r, *r, *r, 255);
    }
    case PixelFormat::kA8: {
      const auto *a = &buffer[base_index * 1];
      return Color(0, 0, 0, *a);
    }
    case PixelFormat::kRGBA8: {
      const auto *RGBA = &buffer[base_index * 4];
      return Color(RGBA[0], RGBA[1], RGBA[2], RGBA[3]);
    }
    case PixelFormat::kFloat32: {
      auto f = reinterpret_cast<const float *>(buffer.data())[base_index];
      return Color(f, f, f, 1);
    }
  }
}

void Texture::GenerateMipMaps() {
  const auto pixel_size = GetPixelSize(pixel_format_);

  auto new_size = size_;
  auto prev_size = size_;
  size_t level = 1;

  while (new_size.Width() > 1 || new_size.Height() > 1) {
    new_size.Width() >>= 1U;
    new_size.Height() >>= 1U;

    if (new_size.Width() < 1) new_size.Width() = 1;
    if (new_size.Height() < 1) new_size.Width() = 1;

    if (level >= levels_.size())
      levels_.emplace_back(new_size.Width() * new_size.Height() * pixel_size);

    if (pixel_format_ != PixelFormat::kFloat32) {
      const auto &down_sampler = down_samplers_.at(pixel_format_);

      size_t pitch = prev_size.Width();

      if (pixel_format_ == PixelFormat::kRGBA8) pitch *= 4;

      down_sampler(prev_size, pitch, levels_[level - 1].data(), levels_[level].data());
    } else
      throw std::runtime_error("no down sampler for pixel format float32");

    prev_size = new_size;
    ++level;
  }
}

Color Texture::Sample(const Sampler &sampler, const Vector2F &coord) const {
  if (!levels_.empty()) {
    float u{}, v{};

    switch (sampler.address_mode_x_) {
      case Sampler::AddressMode::kClamp:u = Clamp<float>(coord.X(), 0, 1);
        break;
      case Sampler::AddressMode::kRepeat:u = float(std::fmod(coord.X(), 1));
        break;
      case Sampler::AddressMode::kMirror:u = 1.0F - float(2.0F * std::fabs(std::fmod(coord.X() / 2, 1)));
        break;
    }

    switch (sampler.address_mode_y_) {
      case Sampler::AddressMode::kClamp:u = Clamp<float>(coord.Y(), 0, 1);
        break;
      case Sampler::AddressMode::kRepeat:u = float(std::fmod(coord.Y(), 1));
        break;
      case Sampler::AddressMode::kMirror:u = 1.0F - float(2.0F * std::fabs(std::fmod(coord.Y() / 2, 1)));
        break;
    }

    u *= float(size_.Width() - 1);
    v *= float(size_.Height() - 1);

    switch (sampler.filter_) {
      case Sampler::Filter::kPoint: {
        const auto texture_x = static_cast<size_t >(std::round(u));
        const auto texture_y = static_cast<size_t >(std::round(v));
        return GetPixel(0, Vector<size_t, 2>{texture_x, texture_y});
      }
      case Sampler::Filter::kLinear: {
        auto texture_x0 = static_cast<size_t >(u - 0.5F);
        auto texture_x1 = texture_x0 + 1;
        auto texture_y0 = static_cast<size_t >(v - 0.5F);
        auto texture_y1 = texture_y0 + 1;

        texture_x0 = Clamp<size_t>(texture_x0, 0, size_.Width() - 1);
        texture_x1 = Clamp<size_t>(texture_x1, 0, size_.Width() - 1);
        texture_y0 = Clamp<size_t>(texture_y0, 0, size_.Height() - 1);
        texture_y1 = Clamp<size_t>(texture_y1, 0, size_.Height() - 1);

        // TODO calculate mip level
        std::array<Color, 4> colors{
            GetPixel(0, Vector<size_t, 2>{texture_x0, texture_y0}),
            GetPixel(0, Vector<size_t, 2>{texture_x1, texture_y0}),
            GetPixel(0, Vector<size_t, 2>{texture_x0, texture_y1}),
            GetPixel(0, Vector<size_t, 2>{texture_x1, texture_y1}),
        };

        const float x0 = u - (texture_x0 + 0.5F);
        const float y0 = v - (texture_y0 + 0.5F);
        const float x1 = (texture_x0 + 1.5F) - u;
        const float y1 = (texture_y0 + 1.5F) - v;

        return colors[0] * x1 * y1 + colors[1] * x0 * y1 + colors[2] * x1 * y0 + colors[3] * x0 * y0;
      }
    }
  }

  return Sample(coord);
}

static void ImageA8DownSampler2x2(const Size2 &size, size_t pitch, const uint8_t *src, uint8_t *dst);
static void ImageR8DownSampler2x2(const Size2 &size, size_t pitch, const uint8_t *src, uint8_t *dst);
static void ImageRGBADownSampler2x2(const Size2 &size, size_t pitch, const uint8_t *src, uint8_t *dst);

std::map<Texture::PixelFormat, DownSampler> Texture::down_samplers_{ // NOLINT(cert-err58-cpp)
    {Texture::PixelFormat::kA8, ImageA8DownSampler2x2},
    {Texture::PixelFormat::kR8, ImageR8DownSampler2x2},
    {Texture::PixelFormat::kRGBA8, ImageRGBADownSampler2x2},
};

void ImageA8DownSampler2x2(const Size2 &size, size_t pitch, const uint8_t *src, uint8_t *dst) {
  const size_t dst_width = size.Width() >> 1U;
  const size_t dst_height = size.Height() >> 1U;

  const size_t y_step = pitch * 2;

  if (dst_width > 0 && dst_height > 0) {
    for (size_t y = 0; y < dst_height; ++y, src += y_step) {
      const uint8_t *pixel = src;
      for (size_t x = 0; x < dst_width; ++x, pixel += 2, dst += 1) {
        float a{};
        a += float(pixel[0]);
        a += float(pixel[1]);
        a += float(pixel[pitch + 0]);
        a += float(pixel[pitch + 1]);
        a /= 4.0F;

        dst[0] = static_cast<uint8_t >(a);
      }
    }
  } else if (dst_height > 0) {
    for (size_t y = 0; y < dst_height; ++y, src += y_step, ++dst) {
      const uint8_t *pixel = src;

      float a{};
      a += float(pixel[0]);
      a += float(pixel[pitch + 0]);
      a /= 2.0F;

      dst[0] = static_cast<uint8_t >(a);
    }
  } else if (dst_width > 0) {
    const uint8_t *pixel = src;

    for (size_t x = 0; x < dst_width; ++x, pixel += 2, ++dst) {
      float a{};
      a += float(pixel[0]);
      a += float(pixel[1]);
      a /= 2.0F;

      dst[0] = static_cast<uint8_t >(a);
    }
  }
}

void ImageR8DownSampler2x2(const Size2 &size, size_t pitch, const uint8_t *src, uint8_t *dst) {
  const size_t dst_width = size.Width() >> 1U;
  const size_t dst_height = size.Height() >> 1U;

  const size_t y_step = pitch * 2;

  if (dst_height > 0 && dst_width > 0) {
    for (size_t y = 0; y < dst_height; ++y, src += y_step) {
      const uint8_t *pixel = src;

      for (size_t x = 0; x < dst_width; ++x, pixel += 2, ++dst) {
        float r{};

        r += std::pow(float(pixel[0]) / 255.0F, Texture::gamma_);
        r += std::pow(float(pixel[1]) / 255.0F, Texture::gamma_);
        r += std::pow(float(pixel[pitch + 0]) / 255.0F, Texture::gamma_);
        r += std::pow(float(pixel[pitch + 1]) / 255.0F, Texture::gamma_);
        r /= 4.0F;

        dst[0] = static_cast<uint8_t >(std::round(std::pow(r, 1.0F / Texture::gamma_) * 255.0F));
      }
    }
  } else if (dst_height > 0) {
    const uint8_t *pixel = src;

    for (size_t y = 0; y < dst_width; ++y, pixel += y_step, ++dst) {
      float r{};

      r += std::pow(float(pixel[0]) / 255.0F, Texture::gamma_);
      r += std::pow(float(pixel[pitch + 0]) / 255.0F, Texture::gamma_);
      r /= 2.0F;

      dst[0] = static_cast<uint8_t >(std::round(std::pow(r, 1.0F / Texture::gamma_) * 255.0F));
    }
  } else if (dst_width > 0) {
    const uint8_t *pixel = src;
    for (size_t x = 0; x < dst_width; ++x, pixel += 2, ++dst) {
      float r{};

      r += std::pow(float(pixel[0]) / 255.0F, Texture::gamma_);
      r += std::pow(float(pixel[1]) / 255.0F, Texture::gamma_);
      r /= 2.0F;

      dst[0] = static_cast<uint8_t >(std::round(std::pow(r, 1.0F / Texture::gamma_) * 255.0F));
    }
  }
}

void ImageRGBADownSampler2x2(const Size2 &size, size_t pitch, const uint8_t *src, uint8_t *dst) {
  const size_t dst_width = size.Width() >> 1U;
  const size_t dst_height = size.Height() >> 1U;

  const size_t y_step = pitch * 2;

  auto common_chan_helper = [&](const uint8_t *pixel,
                                bool use_3 = true,
                                bool use_pitch_plus_3 = true,
                                bool use_7 = false,
                                bool use_pitch_plus_7 = false) -> std::tuple<float, Color> {
    float pixels{};
    Color color{};

    if (use_3 && pixel[3] > 0) {
      color += Vector4F{
          std::pow(float(pixel[0]) / 255.0F, Texture::gamma_),
          std::pow(float(pixel[1]) / 255.0F, Texture::gamma_),
          std::pow(float(pixel[2]) / 255.0F, Texture::gamma_),
          float(pixel[3]),
      };

      pixels += 1.0F;
    }

    if (use_pitch_plus_3 && pixel[pitch + 3] > 0) {
      color += Vector4F{
          std::pow(float(pixel[pitch + 0]) / 255.0F, Texture::gamma_),
          std::pow(float(pixel[pitch + 1]) / 255.0F, Texture::gamma_),
          std::pow(float(pixel[pitch + 2]) / 255.0F, Texture::gamma_),
          float(pixel[3]),
      };

      pixels += 1.0F;
    }

    if (use_7 && pixel[7] > 0) {
      color += Vector4F{
          std::pow(float(pixel[4]) / 255.0F, Texture::gamma_),
          std::pow(float(pixel[5]) / 255.0F, Texture::gamma_),
          std::pow(float(pixel[6]) / 255.0F, Texture::gamma_),
          float(pixel[7])
      };

      pixels += 1.0F;
    }

    if (use_pitch_plus_7 && pixel[pitch + 7] > 0) {
      color += Vector4F{
          std::pow(float(pixel[pitch + 4]) / 255.0F, Texture::gamma_),
          std::pow(float(pixel[pitch + 5]) / 255.0F, Texture::gamma_),
          std::pow(float(pixel[pitch + 6]) / 255.0F, Texture::gamma_),
          float(pixel[pitch + 7])
      };

      pixels += 1.0F;
    }

    return std::make_tuple(pixels, color);
  };

  auto post_helper = [&](float pixels, size_t chan, Color color, uint8_t *dst) {
    if (pixels > 0.0F) {
      auto a_chan = color.A() / chan;
      color /= pixels;

      for (size_t i = 0; i < 3; ++i)
        dst[i] = static_cast<uint8_t >(std::round(std::pow(color[i], 1.0F / Texture::gamma_) * 255.0F));

      dst[3] = static_cast<uint8_t >(a_chan);
    } else {
      for (size_t i = 0; i < 4; i++)
        dst[i] = 0;
    }
  };

  if (dst_width > 0 && dst_height > 0) {
    for (size_t y = 0; y < dst_height; ++y, src += y_step) {
      const uint8_t *pixel = src;
      for (size_t x = 0; x < dst_width; ++x, pixel += 8, dst += 4) {
        auto[pixels, color] = common_chan_helper(pixel, true, true, true, true);

        post_helper(pixels, 4, color, dst);
      }
    }
  } else if (dst_height > 0) {
    for (size_t y = 0; y < dst_height; ++y, src += y_step, dst += 4) {
      const uint8_t *pixel = src;

      auto[pixels, color] = common_chan_helper(pixel);

      post_helper(pixels, 2, color, dst);
    }
  } else if (dst_width > 0) {
    for (size_t y = 0; y < dst_height; ++y, src += y_step, dst += 4) {
      const uint8_t *pixel = src;

      auto[pixels, color] = common_chan_helper(pixel, true, false, true);

      post_helper(pixels, 2, color, dst);
    }
  }
}

} // namespace lutenist