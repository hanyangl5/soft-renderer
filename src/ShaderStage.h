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
	std::array<glm::vec3, 3> view_pos{};
	std::array<glm::vec3, 3> view_normal{};
	std::array<glm::vec2, 3> uv;
};

struct ps_input {
	glm::vec3 view_pos{};
	glm::vec3 view_normal{};
	glm::vec3 view_light_pos{};
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
	return vso;
}

glm::vec3 PixelShader(const ps_input& psin) {

	glm::vec3 light_color{ psin.light_color };
	glm::vec3 view_light_dir{ glm::normalize(psin.view_light_pos - psin.view_pos) };
	glm::vec3 view_dir{ glm::normalize(-psin.view_pos) };
	glm::vec3 half_dir{ glm::normalize(view_dir + view_light_dir) };
	float distance = glm::length(view_light_dir);
	//std::cout << distance << "\n";
	float attenuation = 1.0f / (distance * distance);

	float ka{ 0.05f }, kd{ 0.6f }, ks{ 0.1f };
	
	glm::vec3 albedo{ psin.texture->GetColor(psin.uv,0) };
	glm::vec3 ambient{ ka * albedo };

	glm::vec3 diff{ attenuation*kd * albedo * light_color * std::max(glm::dot(view_light_dir,psin.view_normal),0.0f) };

	glm::vec3 spec{ attenuation*ks * light_color * std::powf(std::max(glm::dot(half_dir,psin.view_normal),0.0f),32) };

	glm::vec3 ret = ambient + diff + spec;
	//ret = glm::vec3(1.0f);

	glm::vec3 color = ret / (ret + glm::vec3(1.0));
	color = glm::pow(color, glm::vec3(1.0 / 2.2));
	return color;


}
//
//glm::vec3 PixelShader(const ps_input& psin, int pbr) {
//	if (!pbr) { return PixelShader(psin); }
//	glm::vec3 view_light_dir{ -glm::normalize(psin.view * psin.light.Dir()) };
//	glm::vec3 view_light_pos{ -glm::normalize(psin.view * psin.light.Pos()) };
//	glm::vec3 view_dir{ -psin.view_pos };
//
//	glm::vec3 N = psin.view_normal;
//	glm::vec3 V = view_dir;
//	glm::vec3 albedo{ psin.texture->GetColor(psin.uv,0) };
//	float metallic = 0.2f, roughness = 0.2f;
//	glm::vec3 Lo = glm::vec3(0.0);
//
//	// calculate per-light radiance
//	//std::cout << psin.view_pos.x<< psin.view_pos.y<< psin.view_pos.z<<"\n";
//	glm::vec3 L = glm::normalize(view_light_pos);
//	glm::vec3 H = glm::normalize(V + L);
//	float distance = glm::length(L);
//	float attenuation = 1.0f / (distance * distance);
//
//	glm::vec3 radiance = psin.light.Color() * attenuation;
//
//	// cook-torrance brdf
//	float ndf = NDF(N, H, roughness, ndf::GGX);
//	float G = GeometrySmith(N, V, L, roughness);
//	glm::vec3 F = Fresnel(IRON, V, H);
//	//F = SILVER;
//
//	glm::vec3 kS = F;
//	glm::vec3 kD = glm::vec3(1.0) - kS;
//	kD *= 1.0 - metallic;
//
//	glm::vec3 numerator = ndf * G * F;
//	float denominator = 4.0 * std::max(glm::dot(N, V), 0.0f) * std::max(glm::dot(N, L), 0.0f);
//	glm::vec3 specular = numerator / std::max(denominator, 0.001f);
//
//	// add to outgoing radiance Lo
//	float NdotL = std::max(dot(N, L), 0.0f);
//	Lo += (kD * albedo / PI + specular) * radiance * NdotL;
//
//
//	glm::vec3 ambient = glm::vec3(0.03) * albedo * 0.2f;// ao
//	glm::vec3 color = ambient + Lo;
//
//	color = color / (color + glm::vec3(1.0));
//	color = glm::pow(color, glm::vec3(1.0 / 2.2));
//
//	return color;
//}