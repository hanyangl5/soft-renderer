#pragma once

#include "Srmath.h"

#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace sr {

class Texture {
private:
  int         u, v, channels;
  BYTE *      data;
  std::string path;

public:
  Texture() = default;
  Texture(int _u, int _v, int _channels, BYTE *_data)
    : u(_u), v(_v), channels(_channels), data(_data) {}
  Texture(const char *Path);
  int         GetU() const { return this->u; }
  int         GetV() const { return this->v; }
  BYTE *      GetData() const { return this->data; }
  srmath::Color GetColor(float _u, float _v);
};

// ctor
Texture::Texture(const char *Path) {
  data = stbi_load(Path, &u, &v, &channels, 4);
  path = Path;
  std::cout << "\ntexture loaded u: " << u << ", v:" << v
            << ", channels:" << channels << "\n";
}

// find color through uv
srmath::Color Texture::GetColor(float _u, float _v) {
  int         i = static_cast<int>(_u * u);
  int         j = static_cast<int>(_v * v);
  srmath::Color c = {data[i * 4 + j * u * 4], data[i * 4 + j * u * 4 + 1],
                   data[i * 4 + j * u * 4 + 2]};
  return c;
}

} // namespace sr