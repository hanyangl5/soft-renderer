#pragma once
#include <array>
#include <glm/glm.hpp>
#include "Light.h"
#include "Texture.h"
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
	std::array<glm::vec4, 3> view_pos{};
	std::array<glm::vec4, 3> view_normal{};
	std::array<glm::vec2, 3> uv;
};

struct ps_input {
	glm::mat4 view{}; //convert light to view space
	glm::vec4 view_pos{};
	glm::vec4 view_normal{};
	DirectLight light{};
	std::shared_ptr<Texture> texture{};
	glm::vec2 uv{};

};

vs_output VertexShader(const vs_input& vsin) {
	vs_output vso{};
	glm::mat4 mv{ vsin.view * vsin.model };
	vso.pos = { vsin.mvp * vsin.vertex[0], vsin.mvp * vsin.vertex[1], vsin.mvp * vsin.vertex[2] };
	vso.view_pos = { mv * vsin.vertex[0], mv * vsin.vertex[1], mv * vsin.vertex[2] };
	vso.view_normal = { glm::normalize(mv * vsin.normal[0]),glm::normalize(mv * vsin.normal[1]), glm::normalize(mv * vsin.normal[2]) };
	vso.uv = vsin.uv;
	return vso;
}

glm::vec3 PixelShader(const ps_input& psin) {

	// compute light in view space, the cam pos is (0,0,0) 

	float ka{ 0.2f }, kd{ 0.5f }, ks{ 0.3f };

	glm::vec3 albedo{ psin.texture->GetColor(psin.uv,0) };
	glm::vec3 ambient{ ka * albedo };
	glm::vec3 light_color{ psin.light.Color() };
	glm::vec4 view_light_dir{ -glm::normalize(psin.view * psin.light.Dir()) };
	glm::vec3 diff{ kd*albedo * light_color * std::max(glm::dot(view_light_dir,psin.view_normal),0.0f) };

	glm::vec4 view_dir{ -psin.view_pos };
	glm::vec4 half_dir{ glm::normalize(view_dir + view_light_dir) };
	glm::vec3 spec{ ks*light_color * std::powf(std::max(glm::dot(half_dir,psin.view_normal),0.0f),32) };

	glm::vec3 ret = ambient + diff + spec;
	//ret = glm::vec3(1.0f);
	return ret;
	

}