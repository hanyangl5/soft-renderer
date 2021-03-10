#pragma once
#include "Srmath.h"

namespace sr {

class DirectLight {
  
  srmath::Vector4f direction;
  // attention! the w value of direction must be 0 to prevent tranlated by model
  // matrix
  srmath::Color color;

public:
  DirectLight(srmath::Vector4f dir, srmath::Color col)
    : direction(srmath::normalize(dir)), color(col) {}
  srmath::Vector4f GetDir() { return direction; }
  srmath::Color    GetColor() { return color; }
  void SetDir(srmath::Vector4f dir) { direction = srmath::normalize(dir); }
};

} // namespace sr