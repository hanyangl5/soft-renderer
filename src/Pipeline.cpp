#include "Pipeline.h"

#include <future>
#include <thread>

constexpr uint32_t coarse_raster_block_x = 8;
constexpr uint32_t coarse_raster_block_y = 8;
static unsigned int max_thread_count = std::thread::hardware_concurrency() - 1;

Pipeline::Pipeline(uint32_t w, uint32_t h) : width(w), height(h) {
    color_buffer.resize(3 * width * height);
    std::fill(color_buffer.begin(), color_buffer.end(), 0);
    depth_buffer.resize(width * height);
    std::fill(depth_buffer.begin(), depth_buffer.end(), 1.0f);
}

void Pipeline::InitAsset() {
    std::filesystem::path asset_path = ASSET_DIR;
    projection = glm::perspective(glm::radians(90.0f), static_cast<float>(width) / height, 0.1f, 100.0f);
    glm::mat4 id(1.0f);

    auto backpack_path = asset_path / "backpack/backpack.obj";
    auto backpack_albedo_path = asset_path / "backpack/diffuse.jpg";
    Model backpack(backpack_path.string());
    backpack.BindAlbedoMap(backpack_albedo_path.string());
    glm::mat4 backpack_t{glm::translate(id, glm::vec3(4.0f, 0.0f, 0.0f))};
    backpack.SetModelMat(backpack_t);

    models.push_back(backpack);
    for (auto model : models) {
        face_count += model.GetTriangleCount();
    }
}

void Pipeline::Update(const std::shared_ptr<Camera> &cam, float delta_time) {
    //std::cout <<face_count<<", "<< face_culled << "\n";
    face_culled = 0;
    view = cam->GetViewMatrix();
    // light rotate by y
    glm::mat4 rot{glm::rotate(glm::mat4(1.0f), glm::radians(0.02f * delta_time), glm::vec3(0.0, 1.0, 0.0))};
    light.Transform(rot);
    //plight.SetRot(rot);

    glm::mat4 rot_plane{
        glm::rotate(models[0].GetModelMat(), glm::radians(1000.0f * 0.2f * delta_time), glm::vec3(0.0, 1.0, 0.0))};

    //models[0].SetModelMat(rot_plane);

    resource.eye_pos = cam->Position;
    resource.view = view;

    resource.light_color = light.Color();
    //psin.view_light_pos = view * plight.Pos();
    resource.light_dir_ws = light.Dir();
}

void Pipeline::Render() {

    vs_input vsin{};
    ps_input psin{};
    for (auto &single_model : models) {

        resource.model = single_model.GetModelMat();
        //std::cout << vsin.model[0][0]<<"\n";
        glm::mat4 mvp = projection * view * single_model.GetModelMat();
        resource.mvp = mvp;
        resource.albedo = single_model.GetAlbedoMap();
        auto &tri_list = single_model.GetTriangleList();
        for (auto &y : tri_list) {
            // input assembly
            vsin.vertex = {y.GetPos(0), y.GetPos(1), y.GetPos(2)};
            vsin.normal = {y.GetNormal(0), y.GetNormal(1), y.GetNormal(2)};
            vsin.uv = {y.GetTexcoord(0), y.GetTexcoord(1), y.GetTexcoord(2)};

            // vertex shader
            // model space to clip space
            vs_output vso = VertexShader(vsin, resource);

            // clipping
            if (!Clipping(vso)) {
                continue;
            }

            // clip to ndc
            PerspectiveDivision(vso);

            if (!CullBackFace(vso)) {
                continue;
            }
            // viewport transformation
            ScreenMapping(vso);

            PerspectiveCorrection(vso);

            // rasterize and pixel processing

            int32_t max_x = std::max({vso.pos[0].x, vso.pos[1].x, vso.pos[2].x});
            int32_t max_y = std::max({vso.pos[0].y, vso.pos[1].y, vso.pos[2].y});
            int32_t min_x = std::min({vso.pos[0].x, vso.pos[1].x, vso.pos[2].x});
            int32_t min_y = std::min({vso.pos[0].y, vso.pos[1].y, vso.pos[2].y});

            //std::vector<> threads;
            for (int32_t i = min_x; i <= max_x; ++i) {
                if (i > width || i < 0) {
                    continue;
                }
                for (int32_t j = min_y; j <= max_y; ++j) {
                    if (j > height || j < 0) {
                        continue;
                    }

                    glm::vec3 bc_coord{ComputeBC(i + 0.5, j + 0.5, vso.pos)};

                    if (bc_coord[0] < 0 || bc_coord[1] < 0 || bc_coord[2] < 0) {
                        continue; // not inside the triangle
                    }

                    float zp{bc_coord[0] * vso.pos[0].z + bc_coord[1] * vso.pos[1].z + bc_coord[2] * vso.pos[2].z};
                    if (zp > depth_buffer[j * width + i]) {
                        continue; // depth test
                    }
                    this->depth_buffer[j * width + i] = zp; // depth write

                    // interp attribute

                    float w =
                        1.0f / (vso.pos[0].w * bc_coord[0] + vso.pos[1].w * bc_coord[1] + vso.pos[2].w * bc_coord[2]);
                    //std::cout << w << "\n";
                    glm::vec3 interp_normal_ws{w * (vso.normal_ws[0] * bc_coord[0] + vso.normal_ws[1] * bc_coord[1] +
                                                    vso.normal_ws[2] * bc_coord[2])};
                    glm::vec3 interp_pos_ws{
                        w * (vso.pos_ws[0] * bc_coord[0] + vso.pos_ws[1] * bc_coord[1] + vso.pos_ws[2] * bc_coord[2])};
                    glm::vec2 interp_uv = {
                        w * (vso.uv[0] * bc_coord[0] + vso.uv[1] * bc_coord[1] + vso.uv[2] * bc_coord[2])};

                    // pixel processing
                    psin.pos_ws = interp_pos_ws;
                    psin.normal_ws = interp_normal_ws;
                    psin.uv = interp_uv;
                    auto result_color = PixelShader(psin, resource);
                    SetPixel(i, j, result_color);
                }
            }
        }
    }
}

void Pipeline::Clear() {
    std::fill(color_buffer.begin(), color_buffer.end(), 0);
    std::fill(depth_buffer.begin(), depth_buffer.end(), 1.0f);
}
void *Pipeline::GetColorBuffer() { return color_buffer.data(); }

bool Pipeline::CullBackFace(const vs_output &vso) {

    glm::vec3 v1{glm::vec3(vso.pos[0])};
    glm::vec3 v2{glm::vec3(vso.pos[1])};
    glm::vec3 v3{glm::vec3(vso.pos[2])};

    glm::vec3 ab{v1 - v2};
    glm::vec3 bc{v2 - v3};
    glm::vec3 view_normal(glm::cross(ab, bc));
    if (glm::dot(view_normal, glm::vec3(0, 0, 1)) <= 0) {
        face_culled++;
        return false;
    }
    return true;
}

bool Pipeline::Clipping(vs_output &vso) {
    for (const auto &i : vso.pos) {
        // check the triangle is inside the frustum
        if (i.x > i.w && i.x < -i.w && i.y > i.w && i.y < -i.w && i.z > i.w && i.z < -i.w) {
            return false;
        }
    }
    return true;
}
void Pipeline::PerspectiveDivision(vs_output &vso) {
    for (auto &i : vso.pos) {
        i.x /= i.w;
        i.y /= i.w;
        i.z /= i.w;
    }
}
void Pipeline::ScreenMapping(vs_output &vso) {
    for (auto &i : vso.pos) {
        i.z = 0.5f * i.z + 0.5f;
        i.x = width * (i.x * 0.5f + 0.5f);
        i.y = 0.5f * height - 0.5f * height * i.y;
    }
}
void Pipeline::PerspectiveCorrection(vs_output &vso) {

    for (uint32_t i = 0; i < 3; ++i) {
        vso.normal_ws[i] /= vso.pos[i].w;
        vso.pos_ws[i] /= vso.pos[i].w;
        vso.uv[i] /= vso.pos[i].w;
        vso.pos[i].w = 1.0f / vso.pos[i].w;
    }
}

glm::vec3 Pipeline::ComputeBC(float x, float y, std::array<glm::vec4, 3> &tri) {
    glm::vec2 v0{tri[1] - tri[0]};            //b-a
    glm::vec2 v1{tri[2] - tri[0]};            //c-a
    glm::vec2 v2{x - tri[0].x, y - tri[0].y}; //p-a

    //Vector v0 = b - a, v1 = c - a, v2 = p - a;
    float d00 = glm::dot(v0, v0);
    float d01 = glm::dot(v0, v1);
    float d11 = glm::dot(v1, v1);
    float d20 = glm::dot(v2, v0);
    float d21 = glm::dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;

    return {u, v, w};
}

void Pipeline::SetPixel(uint32_t x, uint32_t y, const glm::vec3 &color) {
    if (x > width || y > height) {
        return;
    }
    color_buffer[3 * (y * width + x) + 0] = static_cast<char>(255 * color[0]);
    color_buffer[3 * (y * width + x) + 1] = static_cast<char>(255 * color[1]);
    color_buffer[3 * (y * width + x) + 2] = static_cast<char>(255 * color[2]);
}
