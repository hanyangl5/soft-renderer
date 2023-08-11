#pragma once

#include <algorithm>
#include <array>
#include <iostream>

#include <glm/glm.hpp>

#include "Vertex.h"

class Triangle {
  public:
    Triangle() = default;
    void SetVertex(const std::array<glm::vec4, 3> &v) {
        triangle[0].position = v[0];
        triangle[1].position = v[1];
        triangle[2].position = v[2];
    }
    void SetVertex(const glm::vec4 &v1, const glm::vec4 &v2, const glm::vec4 &v3) {
        triangle[0].position = v1;
        triangle[1].position = v2;
        triangle[2].position = v3;
    }
    //void SetAlbedo(glm::vec4 c1, glm::vec4 c2, glm::vec4 c3) {
    //	triangle[0].albedo = c1;
    //	triangle[1].albedo = c2;
    //	triangle[2].albedo = c3;
    //}
    void SetNormal(glm::vec4 &n1, glm::vec4 &n2, glm::vec4 &n3) {
        triangle[0].normal = n1;
        triangle[1].normal = n2;
        triangle[2].normal = n3;
    }
    void SetTexcoord(glm::vec2 &t1, glm::vec2 &t2, glm::vec2 &t3) {
        triangle[0].tex_coord = t1;
        triangle[1].tex_coord = t2;
        triangle[2].tex_coord = t3;
    }
    glm::vec4 GetPos(uint32_t ind) { return triangle[ind].position; }
    glm::vec4 GetNormal(uint32_t ind) { return triangle[ind].normal; }
    glm::vec2 GetTexcoord(uint32_t ind) { return triangle[ind].tex_coord; }

  private:
    std::array<Vertex, 3> triangle{};
};
