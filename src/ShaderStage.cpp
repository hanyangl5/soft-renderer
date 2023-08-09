#include "ShaderStage.h"

vs_output VertexShader(const vs_input &vsin) {
    vs_output vso{};
    glm::mat4 mv{vsin.view * vsin.model};
    vso.pos = {vsin.mvp * vsin.vertex[0], vsin.mvp * vsin.vertex[1], vsin.mvp * vsin.vertex[2]};
    vso.view_pos = {glm::vec3{mv * vsin.vertex[0]}, glm::vec3{mv * vsin.vertex[1]}, glm::vec3{mv * vsin.vertex[2]}};
    vso.view_normal = {glm::normalize(mv * vsin.normal[0]), glm::normalize(mv * vsin.normal[1]),
                       glm::normalize(mv * vsin.normal[2])};
    vso.uv = vsin.uv;
    //std::cout<<glm::to_string(vsin.mvp)<<"\n";
    return vso;
}

glm::vec3 PixelShader(const ps_input &psin) {

    glm::vec3 light_color{psin.light_color};
    //glm::vec3 view_light_dir{ glm::normalize(psin.view_light_pos - psin.view_pos) };
    glm::vec3 view_light_dir = psin.view_light_dir;
    glm::vec3 view_dir{glm::normalize(-psin.view_pos)};
    glm::vec3 half_dir{glm::normalize(view_dir + view_light_dir)};
    //float distance = glm::length(view_light_dir);
    //std::cout << distance << "\n";
    //float attenuation = 1.0f / (distance * distance);

    float ka{0.05f};
    float kd{0.6f};
    float ks{0.1f};

    glm::vec3 albedo = psin.texture->GetColor(psin.uv);
    glm::vec3 ambient{ka * albedo};

    glm::vec3 diff{kd * albedo * light_color * std::max(glm::dot(view_light_dir, psin.view_normal), 0.0f)};

    glm::vec3 spec{ks * light_color * powf(std::max(glm::dot(half_dir, psin.view_normal), 0.0f), 64)};

    glm::vec3 ret = ambient + diff + spec;
    //ret = glm::vec3(1.0f);

    glm::vec3 color = ret / (ret + glm::vec3(1.0));
    color = glm::pow(color, glm::vec3(1.0 / 2.2));
    return color;
}