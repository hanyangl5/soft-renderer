#pragma once
#include "ObJ_Loader.h"
#include <string>
#include <glm/glm.hpp>
#include "Triangle.h"
#include "Texture.h"
class Model {

public:

	Model(std::string path) {
		objl::Loader Loader;
		bool loadout = Loader.LoadFile(path);
		if (!loadout)
			std::cout << "failed to load\n";
		
			uint32_t v_count{ 0 };
		for (auto mesh : Loader.LoadedMeshes) {
			for (int i = 0; i < mesh.Vertices.size(); i += 3) {
				Triangle t;
				glm::vec4 v1{ mesh.Vertices[i + 0].Position.X, mesh.Vertices[i + 0].Position.Y, mesh.Vertices[i + 0].Position.Z,1.0f };
				glm::vec4 v2{ mesh.Vertices[i + 1].Position.X, mesh.Vertices[i + 1].Position.Y, mesh.Vertices[i + 1].Position.Z,1.0f };
				glm::vec4 v3{ mesh.Vertices[i + 2].Position.X, mesh.Vertices[i + 2].Position.Y, mesh.Vertices[i + 2].Position.Z,1.0f };
				
				glm::vec4 n1{ mesh.Vertices[i + 0].Normal.X, mesh.Vertices[i + 0].Normal.Y, mesh.Vertices[i + 0].Normal.Z,0.0f };
				glm::vec4 n2{ mesh.Vertices[i + 1].Normal.X, mesh.Vertices[i + 1].Normal.Y, mesh.Vertices[i + 1].Normal.Z,0.0f };
				glm::vec4 n3{ mesh.Vertices[i + 2].Normal.X, mesh.Vertices[i + 2].Normal.Y, mesh.Vertices[i + 2].Normal.Z,0.0f };

				glm::vec2 t1{ mesh.Vertices[i + 0].TextureCoordinate.X, mesh.Vertices[i + 0].TextureCoordinate.Y};
				glm::vec2 t2{ mesh.Vertices[i + 1].TextureCoordinate.X, mesh.Vertices[i + 1].TextureCoordinate.Y};
				glm::vec2 t3{ mesh.Vertices[i + 2].TextureCoordinate.X, mesh.Vertices[i + 2].TextureCoordinate.Y};

				t.SetVertex(v1, v2, v3);
				t.SetNormal(n1, n2, n3);
				t.SetTexcoord(t1, t2, t3);
				triangle_list.push_back(t);
			}
			v_count += mesh.Vertices.size();
		}
		std::cout << path << " vertex count: "<<v_count<<"\n";
	}
	std::vector<Triangle> GetTriangleList() {
		return triangle_list;
	}
	void SetModelMat(const glm::mat4& t) {
		translate = t;
	}
	glm::mat4 GetModelMat() {
		return translate;
	}

	uint32_t GetTriangleCount() {
		return static_cast<uint32_t>(triangle_list.size());
	}
	std::shared_ptr<Texture> GetTexture() { return texture; }
	void BindTexture(std::string path) {
		texture = std::make_shared<Texture>(path,0);
	}
private:
	std::shared_ptr<Texture> texture{};// only 1 texture
	std::vector<Triangle> triangle_list{};
	glm::mat4 translate{glm::mat4(1.0f)};
};
