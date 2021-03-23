#pragma once
#include <glm/glm.hpp>
class DirectLight {
public:
	DirectLight() = default;
	DirectLight(glm::vec4 pos,glm::vec4 dir,glm::vec3 col):light_pos(pos),light_dir(dir),light_color(col) {};

	glm::vec4 Dir()const{ return light_dir; }
	glm::vec3 Color()const { return light_color; }
	glm::vec4 Pos()const { return light_pos; }
	void Transform(const glm::mat4& mat) {
		light_model_mat=mat*light_model_mat;
		light_dir = light_model_mat * light_dir;
		light_pos = light_model_mat * light_pos;
	}
private:
	glm::vec4 light_pos{};
	glm::vec4 light_dir{};
	glm::vec3 light_color{};
	glm::mat4 light_model_mat{ glm::mat4{1.0f} };
};