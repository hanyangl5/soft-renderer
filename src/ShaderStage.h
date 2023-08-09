#pragma once
#include <array>
#include <memory>

#include <glm/glm.hpp>

#include "Light.h"
#include "Texture.h"

//#include <glm/gtx/string_cast.hpp>
//#include "Pipeline.h"

// uniform buffer
struct shader_resource {
    // vs
    glm::mat4 model{};
    glm::mat4 view{};
    glm::mat4 mvp{};
    // ps
    glm::vec3 eye_pos{};
    std::shared_ptr<Texture> albedo{};

    glm::vec3 light_pos_ws{};
    glm::vec3 light_dir_ws{};
    glm::vec3 light_color{};
};
struct vs_input {
    std::array<glm::vec4, 3> vertex{};
    std::array<glm::vec4, 3> normal{};
    std::array<glm::vec2, 3> uv{};
};

struct vs_output {
    std::array<glm::vec4, 3> pos{};
    std::array<glm::vec3, 3> pos_ws{};
    std::array<glm::vec3, 3> normal_ws{};
    std::array<glm::vec2, 3> uv;
};

struct ps_input {
    glm::vec3 pos_ws{};
    glm::vec3 normal_ws{};
    glm::vec2 uv{};
};

vs_output VertexShader(const vs_input &vsin, shader_resource& resource);
glm::vec3 PixelShader(const ps_input &psin, shader_resource &resource);