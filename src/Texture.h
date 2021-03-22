#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//#include <stb_image_resize.h>
//#include <stb_image_write.h>
#include <string>
#include <array>
#include <vector>
#include <cstdint>
#include <cmath>
inline float interp(float x, float y, float t)
{
	return x * t + y * t;
}

class Texture
{
public:
	Texture(std::string _path, uint32_t mipmap_level = 3)
	{
		path = _path;

		unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 4);
		if (!data)
			std::cout << "failed to load texture\n";
		color_data.assign(data, data + width * height * 4);

		if (mipmap_level)
			GenerateMipmap("", mipmap_level);
	}

	void GenerateMipmap(std::string output_dir, uint32_t mipmap_level)
	{
		//for (uint32_t i = 0; i < mipmap_level;++i){
		//    std::string mipmap_path{};
		//}
	}

	glm::vec3 GetColor(glm::vec2 uv, float distance = 0)
	{
		// model does not have texture
		if (color_data.size() == 0)
		{
			return {1.0, 1.0, 1.0};
		}
		// bilinear texture filtering
		float interp_u = uv[0] - std::floor(uv[0]);
		float interp_v = uv[1] - std::floor(uv[1]);
		int lu = std::floor(width * uv[0]);
		int tv = std::floor(height * uv[1]);
		int ru = std::ceil(width * uv[0]);
		int dv = std::ceil(height * uv[1]);
		glm::vec3 rt{static_cast<float>(color_data[ru * 4 + tv * width * 4]+0) / 255.0f,
					 static_cast<float>(color_data[ru * 4 + tv * width * 4 + 1]) / 255.0f,
					 static_cast<float>(color_data[ru * 4 + tv * width * 4 + 2]) / 255.0f };
		glm::vec3 lt{static_cast<float>(color_data[lu * 4 + tv * width * 4]) / 255.0f,
					 static_cast<float>(color_data[lu * 4 + tv * width * 4 + 1]) / 255.0f,
					 static_cast<float>(color_data[lu * 4 + tv * width * 4 + 2]) / 255.0f };
		glm::vec3 rd{static_cast<float>(color_data[ru * 4 + dv * width * 4]) / 255.0f,
					 static_cast<float>(color_data[ru * 4 + dv * width * 4 + 1]) / 255.0f,
					 static_cast<float>(color_data[ru * 4 + dv * width * 4 + 2]) / 255.0f };
		glm::vec3 ld{static_cast<float>(color_data[lu * 4 + dv * width * 4]) / 255.0f,
					 static_cast<float>(color_data[lu * 4 + dv * width * 4 + 1]) / 255.0f,
					 static_cast<float>(color_data[lu * 4 + dv * width * 4 + 2]) / 255.0f };
		

		glm::vec3 interp_1{ interp_u * rt + (1.0f - interp_u) * lt };
		glm::vec3 interp_2{ interp_u * rd + (1.0f - interp_u) * ld };
		
		glm::vec3 res{interp_v*interp_1+(1.0f-interp_v)*interp_2};
		//res = ld;
		return res;
	}

private:
	int width, height, channels;
	std::string path{};
	std::vector<unsigned char> color_data{};
};