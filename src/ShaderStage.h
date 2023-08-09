#pragma once
#include <array>
#include <memory>

#include <glm/glm.hpp>

#include "Light.h"
#include "Texture.h"

//#include <glm/gtx/string_cast.hpp>
//#include "Pipeline.h"

struct vs_input {
    glm::mat4 model{};
    glm::mat4 view{};
    glm::mat4 mvp{};

    std::array<glm::vec4, 3> vertex{};
    std::array<glm::vec4, 3> normal{};
    std::array<glm::vec2, 3> uv{};
};

struct vs_output {
    std::array<glm::vec4, 3> pos{};
    std::array<glm::vec3, 3> view_pos{};
    std::array<glm::vec3, 3> view_normal{};
    std::array<glm::vec2, 3> uv;
};

struct ps_input {
    glm::vec3 view_pos{};
    glm::vec3 view_normal{};
    glm::vec3 view_light_pos{};
    glm::vec3 view_light_dir{};
    glm::vec3 light_color{};
    glm::vec2 uv{};
    std::shared_ptr<Texture> texture{};
};

vs_output VertexShader(const vs_input &vsin);
glm::vec3 PixelShader(const ps_input &psin);