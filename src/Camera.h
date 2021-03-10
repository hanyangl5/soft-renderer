#pragma once

#include "Srmath.h"
#include "WindowClass.h"

namespace sr {

class Camera {

private:
  srmath::Vector4f position, at, roll, forward, pitch, yaw;
  srmath::Matrix4f view;

public:
Camera() = default;
  Camera(srmath::Vector4f pos, srmath::Vector4f at);
  void           CameraRotate(int x, int y);
  void           CameraMove(int dir, int count); // 0 :roll 1 pitch 2 yaw
  srmath::Vector4f GetDirection() const;
  srmath::Vector4f GetPos() const;
  srmath::Matrix4f GetViewMatrix() const;
  void           SetViewMatrix() {
    this->view << pitch.x, pitch.y, pitch.z, -dot(pitch, position), yaw.x,
      yaw.y, yaw.z, -dot(yaw, position), roll.x, roll.y, roll.z,
      -dot(roll, position);
    this->view;
  }
};

// initialize view matrix through camera
Camera::Camera(srmath::Vector4f pos, srmath::Vector4f _at)
  : position(pos), at(_at), roll(normalize(position - at)), forward(-roll),
    pitch(normalize(cross(srmath::Vector4f(0, 1, 0), roll))),
    yaw(normalize(cross(roll, pitch))) {
  SetViewMatrix();
}

// rotate camera by changing mouse position
void Camera::CameraRotate(int x, int y) {
  // x :rotate by yaw
  // y :rotate by pitch
  float          speed = 1;
  srmath::Matrix4f ry, rx;
  //std::cout << x << y << "\n";
  yaw = ry.Rotate(pitch, (double)y * speed / window::WINDOW_WIDTH) * yaw;
  roll = ry.Rotate(pitch, (double)y * speed / window::WINDOW_WIDTH) * roll;
  pitch = rx.Rotate(yaw, (double)x * speed / window::WINDOW_HEIGHT) * pitch;
  roll = rx.Rotate(yaw, (double)x * speed / window::WINDOW_HEIGHT) * roll;
  forward = -roll;
  SetViewMatrix();
}

// move camera
void Camera::CameraMove(int dir, int sign) {
  float speed = 0.05;
  switch (dir) {
  case 1: {
    position += forward * sign * speed;
    break;
  }
  case 2: {
    position -= pitch * sign * speed;
    break;
  }
  default:
    break;
  }
  SetViewMatrix();
}

// get cam dir
srmath::Vector4f Camera::GetDirection() const { return this->forward; }

// get cam pos
srmath::Vector4f Camera::GetPos() const { return this->position; }

// get view mat
srmath::Matrix4f Camera::GetViewMatrix() const { return this->view; }


// camera movement
void ProcessKeyInput(sr::Camera& camera) {
    // w move forward
    if (sr::window::screen_keys[0x57]) {
        camera.CameraMove(1, 1);
    }
    // s move back
    if (sr::window::screen_keys[0x53]) {
        camera.CameraMove(1, -1);
    }
    // a move left
    if (sr::window::screen_keys[0x41]) {
        camera.CameraMove(2, 1);
    }
    // d move right
    if (sr::window::screen_keys[0x44]) {
        camera.CameraMove(2, -1);
    }


    // up
    if (sr::window::screen_keys[VK_UP]) {
        camera.CameraMove(1, 1);
    }
    // down
    if (sr::window::screen_keys[VK_DOWN]) {
        camera.CameraMove(1, -1);
    }
    // left
    if (sr::window::screen_keys[VK_LEFT]) {
        camera.CameraMove(2, 1);
    }
    // right
    if (sr::window::screen_keys[VK_RIGHT]) {
        camera.CameraMove(2, -1);
    }
}


} // namespace sr