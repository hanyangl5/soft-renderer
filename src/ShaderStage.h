#pragma once
#include <array>
#include <glm/glm.hpp>
#include "Light.h"
#include "Texture.h"
#include <glm/gtx/string_cast.hpp>
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

vs_output VertexShader(const vs_input& vsin) {
	vs_output vso{};
	glm::mat4 mv{ vsin.view * vsin.model };
	vso.pos = { vsin.mvp * vsin.vertex[0], vsin.mvp * vsin.vertex[1], vsin.mvp * vsin.vertex[2] };
	vso.view_pos = { glm::vec3{mv * vsin.vertex[0]}, glm::vec3{mv * vsin.vertex[1]}, glm::vec3{mv * vsin.vertex[2]} };
	vso.view_normal = { glm::normalize(mv * vsin.normal[0]),glm::normalize(mv * vsin.normal[1]), glm::normalize(mv * vsin.normal[2]) };
	vso.uv = vsin.uv;
	//std::cout<<glm::to_string(vsin.mvp)<<"\n";
	return vso;
}

glm::vec3 PixelShader(const ps_input& psin) {

	glm::vec3 light_color{ psin.light_color };
	//glm::vec3 view_light_dir{ glm::normalize(psin.view_light_pos - psin.view_pos) };
	glm::vec3 view_light_dir = psin.view_light_dir;
	glm::vec3 view_dir{ glm::normalize(-psin.view_pos) };
	glm::vec3 half_dir{ glm::normalize(view_dir + view_light_dir) };
	//float distance = glm::length(view_light_dir);
	//std::cout << distance << "\n";
	//float attenuation = 1.0f / (distance * distance);

	float ka{ 0.05f }, kd{ 0.6f }, ks{ 0.1f };
	
	glm::vec3 albedo{ psin.texture->GetColor(psin.uv,0) };
	glm::vec3 ambient{ ka * albedo };

	glm::vec3 diff{ kd * albedo * light_color * std::max(glm::dot(view_light_dir,psin.view_normal),0.0f) };

	glm::vec3 spec{ ks * light_color * std::powf(std::max(glm::dot(half_dir,psin.view_normal),0.0f),64) };

	glm::vec3 ret = ambient + diff + spec;
	//ret = glm::vec3(1.0f);

	glm::vec3 color = ret / (ret + glm::vec3(1.0));
	color = glm::pow(color, glm::vec3(1.0 / 2.2));
	return color;


}