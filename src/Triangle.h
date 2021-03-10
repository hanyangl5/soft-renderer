#pragma once

#include "Srmath.h"
#include <algorithm>
#include <array>
#include <iostream>

namespace sr {

class Triangle {
public:
  std::array<srmath::Vector4f, 3> vertex;
  std::array<srmath::Color, 3>    color;
  std::array<srmath::Vector4f, 3> normal;
  std::array<srmath::Vector2f, 3> tex_coords;

  Triangle();

  void SetVertex(int ind, srmath::Vector4f ver);
  void SetVertex(srmath::Vector4f v1, srmath::Vector4f v2, srmath::Vector4f v3);
  void SetColor(srmath::Color v1, srmath::Color v2, srmath::Color v3);
  void SetNormal(srmath::Vector4f v1, srmath::Vector4f v2, srmath::Vector4f v3);
  void SetTexcoord(int ind, srmath::Vector2f uv);
  void SetTexcoord(srmath::Vector2f v1, srmath::Vector2f v2, srmath::Vector2f v3);
};

// ctor
Triangle::Triangle() {
  vertex.at(0) = {0., 0., 0., 1.};
  vertex.at(1) = {0., 0., 0., 1.};
  vertex.at(2) = {0., 0., 0., 1.};
  color.at(0) = {0, 0, 0};
  color.at(1) = {0, 0, 0};
  color.at(2) = {0, 0, 0};
  normal.at(0) = {0., 0., 0., 1.};
  normal.at(1) = {0., 0., 0., 1.};
  normal.at(2) = {0., 0., 0., 1.};
  tex_coords.at(0) = {0.0f, 0.0f};
  tex_coords.at(1) = {0.0f, 0.0f};
  tex_coords.at(2) = {0.0f, 0.0f};
}

// set vertex
void Triangle::SetVertex(int index, srmath::Vector4f ver) {
  this->vertex[index] = ver;
}

// set vertex
void Triangle::SetVertex(srmath::Vector4f v1, srmath::Vector4f v2,
                         srmath::Vector4f v3) {
  this->vertex.at(0) = v1;
  this->vertex.at(1) = v2;
  this->vertex.at(2) = v3;
}

// set vertex color
void Triangle::SetColor(srmath::Color v1, srmath::Color v2, srmath::Color v3) {
  this->color.at(0) = v1;
  this->color.at(1) = v2;
  this->color.at(2) = v3;
}

// set normal of each vertex
void Triangle::SetNormal(srmath::Vector4f v1, srmath::Vector4f v2,
                         srmath::Vector4f v3) {
  this->normal.at(0) = v1;
  this->normal.at(1) = v2;
  this->normal.at(2) = v3;
}

// set vertex texcoord
void Triangle::SetTexcoord(int ind, srmath::Vector2f uv) {
  this->tex_coords[ind] = uv;
}

// set vertex texcoord
void Triangle::SetTexcoord(srmath::Vector2f v1, srmath::Vector2f v2,
                           srmath::Vector2f v3) {
  this->tex_coords.at(0) = v1;
  this->tex_coords.at(1) = v2;
  this->tex_coords.at(2) = v3;
}

} // namespace sr