#pragma once

#include "Srmath.h"
#include "Texture.h"
#include "Triangle.h"
#include "OBJ_Loader.h"
#include <vector>

namespace sr {

void LoadModel(std::string Path, std::vector<srmath::Vector4f> &vert,
               std::vector<srmath::Indices> &ind) {}

class Scene {
private:
  srmath::Vector4f position;
  float          scale;
  srmath::Matrix4f model;

public:
  Scene(const std::string &Path);
  void                    SetRotate(srmath::Vector4f axis, float angle);
  void                    SetTranslate(const srmath::Vector4f t);
  void                    SetScale(srmath::Vector4f s);
  void                    SetScale(float s);
  srmath::Matrix4f          GetModelMatrix() const;
  void                    BindTexture(const Texture &tex);
  std::vector<Triangle *> triangle_list;
  Texture                 texture;
};

// load model from file
Scene::Scene(const std::string &Path)
  : scale(1), position(srmath::Vector4f(0, 0, 0)) {
  objl::Loader Loader;

  bool loadout = Loader.LoadFile(Path);

  if (!loadout)
    std::cout << "failed to load\n";

  for (auto mesh : Loader.LoadedMeshes) {
    for (int i = 0; i < mesh.Vertices.size(); i += 3) {
      Triangle *t = new Triangle();
      t->SetVertex(srmath::Vector4f(mesh.Vertices[i + 0].Position.X,
                                  mesh.Vertices[i + 0].Position.Y,
                                  mesh.Vertices[i + 0].Position.Z),
                   srmath::Vector4f(mesh.Vertices[i + 1].Position.X,
                                  mesh.Vertices[i + 1].Position.Y,
                                  mesh.Vertices[i + 1].Position.Z),
                   srmath::Vector4f(mesh.Vertices[i + 2].Position.X,
                                  mesh.Vertices[i + 2].Position.Y,
                                  mesh.Vertices[i + 2].Position.Z));

      t->SetNormal(srmath::Vector4f(mesh.Vertices[i + 0].Normal.X,
                                  mesh.Vertices[i + 0].Normal.Y,
                                  mesh.Vertices[i + 0].Normal.Z, 0),
                   srmath::Vector4f(mesh.Vertices[i + 1].Normal.X,
                                  mesh.Vertices[i + 1].Normal.Y,
                                  mesh.Vertices[i + 1].Normal.Z, 0),
                   srmath::Vector4f(mesh.Vertices[i + 2].Normal.X,
                                  mesh.Vertices[i + 2].Normal.Y,
                                  mesh.Vertices[i + 2].Normal.Z, 0));
      t->SetTexcoord(srmath::Vector2f(mesh.Vertices[i + 0].TextureCoordinate.X,
                                    mesh.Vertices[i + 0].TextureCoordinate.Y),
                     srmath::Vector2f(mesh.Vertices[i + 1].TextureCoordinate.X,
                                    mesh.Vertices[i + 1].TextureCoordinate.Y),
                     srmath::Vector2f(mesh.Vertices[i + 2].TextureCoordinate.X,
                                    mesh.Vertices[i + 2].TextureCoordinate.Y));

      triangle_list.push_back(t);
    }
    std::cout << "vert count: " << mesh.Vertices.size()
              << ", face count: " << mesh.Indices.size();
  }
}

// model matrix roatation
void Scene::SetRotate(srmath::Vector4f axis, float angle) {
  this->model = model.Rotate(axis, angle) * this->model;
}

// model matrix tranlation
void Scene::SetTranslate(const srmath::Vector4f t) {
  this->model = model.Translate(t) * this->model;
}

// set scale by 3 axis
void Scene::SetScale(srmath::Vector4f s) {
  this->model = model.Scale(s) * this->model;
}

// set scale by 1 parameter
void Scene::SetScale(float s) { this->model = model.Scale(s) * this->model; }

// get model mat
srmath::Matrix4f Scene::GetModelMatrix() const { return this->model; }

// bind texture
void Scene::BindTexture(const Texture &tex) { this->texture = tex; }

} // namespace sr