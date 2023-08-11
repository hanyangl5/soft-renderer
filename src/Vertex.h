#pragma once
#include <glm/glm.hpp>

struct Vertex {
    glm::vec4 position{};
    glm::vec3 albedo{};
    glm::vec4 normal{};
    glm::vec2 tex_coord{};
};
