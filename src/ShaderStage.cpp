#include "ShaderStage.h"

vs_output VertexShader(const vs_input &vsin, shader_resource& resource) {
    vs_output vso{};
    glm::mat4 mv = resource.view * resource.model;
    vso.pos = {resource.mvp * vsin.vertex[0], resource.mvp * vsin.vertex[1], resource.mvp * vsin.vertex[2]};
    vso.pos_ws = {glm::vec3{resource.model * vsin.vertex[0]}, glm::vec3{resource.model * vsin.vertex[1]},
                  glm::vec3{resource.model * vsin.vertex[2]}};
    vso.normal_ws = {glm::normalize(resource.model * vsin.normal[0]), glm::normalize(resource.model * vsin.normal[1]),
                       glm::normalize(resource.model * vsin.normal[2])};
    vso.uv = vsin.uv;
    //std::cout<<glm::to_string(vsin.mvp)<<"\n";
    return vso;
}

glm::vec3 PixelShader(const ps_input &psin, shader_resource &resource) {

    glm::vec3 light_color{resource.light_color};
    glm::vec3 light_dir = resource.light_dir_ws;
    glm::vec3 view_dir{glm::normalize(resource.eye_pos- psin.pos_ws)};
    glm::vec3 half_dir{glm::normalize(light_dir + view_dir)};

    float ka{0.05f};
    float kd{0.6f};
    float ks{0.1f};

    glm::vec3 albedo = resource.albedo->Sample(psin.uv);
    glm::vec3 ambient{ka * albedo};

    glm::vec3 diff{kd * albedo * light_color * std::max(glm::dot(light_dir, psin.normal_ws), 0.0f)};

    glm::vec3 spec{ks * light_color * powf(std::max(glm::dot(half_dir, psin.normal_ws), 0.0f), 64)};

    glm::vec3 ret = ambient + diff + spec;

    glm::vec3 color = ret / (ret + glm::vec3(1.0));
    color = glm::pow(color, glm::vec3(1.0 / 2.2));
    return color;
}