#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "OBJ_Loader.h"
#include "Texture.h"
#include "Triangle.h"

class Model {
  public:
    explicit Model(const std::string &path);
    std::vector<Triangle> &GetTriangleList();
    void SetModelMat(const glm::mat4 &t);
    glm::mat4 &GetModelMat();

    uint32_t GetTriangleCount();
    std::shared_ptr<Texture> &GetAlbedoMap();
    void BindAlbedoMap(const std::string &path);

  private:
    std::shared_ptr<Texture> albedo{}; // only 1 texture
    std::vector<Triangle> triangle_list{};
    glm::mat4 translate{glm::mat4(1.0f)};
};
