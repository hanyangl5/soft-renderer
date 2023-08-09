#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "ShaderStage.h"
#include "config.hpp"

template <typename T> struct atomwrapper {
    std::atomic<T> _a;

    atomwrapper() : _a() {}

    atomwrapper(atomwrapper &&) = delete;
    atomwrapper &operator=(atomwrapper &&) = delete;
    explicit atomwrapper(const std::atomic<T> &a) : _a(a.load()) {}

    atomwrapper(const atomwrapper &other) : _a(other._a.load()) {}

    atomwrapper &operator=(const atomwrapper &other) { _a.store(other._a.load()); }
};

class Pipeline {
  public:
    Pipeline(uint32_t w, uint32_t h);
    ~Pipeline() = default;

    Pipeline(const Pipeline &) = delete;
    Pipeline(Pipeline &&) = delete;
    Pipeline &operator=(const Pipeline &) = delete;
    Pipeline &operator=(Pipeline &&) = delete;

    void InitAsset();
    void Update(const std::shared_ptr<Camera> &cam, float delta_time);
    void Render();
    void Clear();
    void *GetColorBuffer();
    //auto GetInfo() { return face_count, face_culled; }

  private:
    bool CullBackFace(const vs_output &vso);
    bool Clipping(vs_output &vso);
    void PerspectiveDivision(vs_output &vso);
    void ScreenMapping(vs_output &vso);
    void PerspectiveCorrection(vs_output &vso);

    glm::vec3 ComputeBC(float x, float y, std::array<glm::vec4, 3> &tri);

    void SetPixel(uint32_t x, uint32_t y, const glm::vec3 &color);

    shader_resource resource;
    uint32_t width{}, height{};
    std::vector<unsigned char> color_buffer{};
    std::vector<float> depth_buffer{};
    glm::mat4 view{}, projection{};
    std::vector<Model> models{};
    // pos, dir, color
    DirectLight light{glm::vec4(0.0f, -0.3f, -1.0f, 0.0f), 2.0f * glm::vec3(1.0f, 0.956f, 0.8392f)};
    //PointLight plight{ glm::vec4(0.0f,5.0f,2.0f,1.0f),glm::vec3(1.0f,0.956f,0.8392f) };
    uint32_t face_count{0}, face_culled{0};
};