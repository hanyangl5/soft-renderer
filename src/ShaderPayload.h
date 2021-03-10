#pragma once
#include <vector>
#include "DirectLight.h"
#include "Srmath.h"
#include "Texture.h"
#include <algorithm>
#define NOMINMAX
namespace sr {

	struct fragment_shader_payload {

		fragment_shader_payload(const srmath::Color& col, const srmath::Vector4f& nor,
			const srmath::Vector2f tc, Texture* tex)
			: color(col), normal(nor), texcoord(tc), texture(tex) {}

		srmath::Vector4f             view_pos;
		srmath::Color                color;
		srmath::Vector4f             normal;
		srmath::Vector2f             texcoord;
		Texture* texture;
		srmath::Vector4f view_dir;
		std::vector<DirectLight*> light_list;
	};

	// texture mapping shader
	srmath::Color texture_shader(fragment_shader_payload payload) {
		float       u = std::clamp(static_cast<double>(payload.texcoord.u), 0.0, 1.0);
		float       v = std::clamp(static_cast<double>(payload.texcoord.v), 0.0, 1.0);
		srmath::Color return_color = payload.texture->GetColor(u, v);
		return return_color;
	}

	// normal shader
	srmath::Color normal_shader(fragment_shader_payload payload) {
		auto s = (srmath::normalize(payload.normal) + srmath::Vector4f(1, 1, 1)) / 2;
		return srmath::Color{ static_cast<unsigned int>(255 * s.x), static_cast<unsigned int>(255 * s.y),
						   static_cast<unsigned int>(255 * s.z) };
	}

	// phong shader (unfinished)
	srmath::Color phong_shader(fragment_shader_payload payload) {
		//srmath::Vector4f ka = srmath::Vector4f(0.005, 0.005, 0.005);
		//srmath::Color    kd = payload.color;
		//srmath::Vector4f ks = srmath::Vector4f(0.7937, 0.7937, 0.7937);

		srmath::Vector4f amb_light_intensity{ 10, 10, 10 };
		srmath::Vector4f eye_pos{ 0, 0, 10 };

		float p = 150;

		srmath::Color    color = payload.color;
		srmath::Vector4f point = payload.view_pos;
		srmath::Vector4f normal = srmath::normalize(payload.normal);
		/*srmath::Color    result_color = { 0, 0, 0 };*/

		srmath::Vector4f light_dir = { -1,-1,-1 ,0 };
		light_dir = srmath::normalize(light_dir);
		srmath::Vector4f light_ambient = { 0.2,0.2,0.2 };
		srmath::Vector4f light_diffuse = { 0.5,0.5,0.5 };
		srmath::Vector4f light_specular = { 1.0,1.0,1.0 };

		srmath::Vector4f ka = {0.2,0.2,0.2 };
		srmath::Vector4f kd = {0.5,0.5,0.5 };
		srmath::Vector4f ks = {1.0,1.0,1.0 };

		srmath::Vector4f ambient = light_ambient * ka;

		srmath::Vector4f diffuse = light_diffuse * kd* std::max(0.0f, srmath::dot(-normal, light_dir));

		srmath::Vector4f view_dir = payload.view_dir;
		//	srmath::normalize(payload.view_pos - srmath::Vector4f(2, 2, 5));
		srmath::Vector4f h = -srmath::normalize(view_dir + light_dir);
		srmath::Vector4f    spec =std::pow(std::max(0.0f, srmath::dot(normal, h)), 32) * light_diffuse;
		//std::cout << normal<<"   "<<h<< "\n";
		//std::cout << std::max(0.0f, srmath::dot(normal, h))<< "\n";
		srmath::Vector4f light_col = ambient + spec+diffuse;
		srmath::Color  result_color = { static_cast<unsigned int>(light_col.x*255), static_cast<unsigned int>(light_col.y * 255), static_cast<unsigned int>(light_col.z * 255) };

		float       u = std::clamp(static_cast<double>(payload.texcoord.u), 0.0, 1.0);
		float       v = std::clamp(static_cast<double>(payload.texcoord.v), 0.0, 1.0);
		srmath::Color tex_color = payload.texture->GetColor(u, v);
		

		std::clamp((int)result_color.x, 0, 255);
		std::clamp((int)result_color.y, 0, 255);
		std::clamp((int)result_color.z, 0, 255);
		return result_color * tex_color;
	}

} // namespace sr