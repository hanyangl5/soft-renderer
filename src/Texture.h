#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

class Texture {
  public:
    Texture(const std::string &path);

    glm::vec3 GetColor(const glm::vec2& uv);

  private:
    int width, height, channels;
    std::string path{};
    std::vector<unsigned char> color_data{};
};