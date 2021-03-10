#pragma once

#include "Camera.h"
#include "DirectLight.h"

#include "Srmath.h"
#include "Scene.h"
#include "Texture.h"
#include "Triangle.h"
#include "ShaderPayload.h"


#include <algorithm> 
#include <iostream>
#include <string>
#include <vector>


namespace sr {

	// rendermodes
	enum RENDERMODE { WIREFRAME = 1, NORMAL = 2, TEXTURE = 3, PHONG = 4 };

	class Pipeline {
	private:
		const unsigned int         width, height;
		BYTE** frame_buffer{};
		std::vector<double>        z_buffer;
		std::vector<Scene*>       object_list;
		std::vector<DirectLight*> light_list;
		srmath::Matrix4f             view, projection;
		Camera cam;
		int                        rendermode = 0;

	public:
		Pipeline(HDC screen_dc, unsigned int w, unsigned int h);
		srmath::Matrix4f GetModelMatrix(const Scene& scene) const;
		srmath::Matrix4f GetViewMatrix(const Camera& camera) const;
		srmath::Matrix4f GetPerspectiveProjectionMatrix(const float eye_fov,
			const float aspect_ratio,
			const float z_near,
			const float z_far) const;

		// write color to frame_buffer
		void SetPixel(const int x, const int y,
			const srmath::Color& color = { 255, 255, 255 });
		void DrawLine(int x0, int y0, int x1, int y1, const srmath::Color& color);
		//void RasterizeScanLine(int x0, int x1, int y, const srmath::Color& color);
		void RasterizeWireframe(const Triangle& t, const srmath::Color& color);
		void Draw(const Camera& camera, int _rendermode);
		void DrawObject(Scene& scene);
		void ClearBuffer();
		int  GetIndex(int x, int y);
		void DrawTriangle(Triangle& t, std::vector<srmath::Vector4f>& view_space_pos,
			Texture& tex,srmath::Vector4f view_dir);
		void AddToPipeline(Scene& obj) { object_list.push_back(&obj); }
		void AddLight(DirectLight& light) { light_list.push_back(&light); }
		void ClearObject() { object_list.clear(); }
		void RemoveObject(int index) {
			if (object_list.size() == 0)
				return;
			auto iter = object_list.begin();
			object_list.erase(iter + index);
		}
	};

	// ctor
	Pipeline::Pipeline(HDC screen_dc, unsigned int w, unsigned int h)
		: width(w), height(h) {

		BITMAPINFO* Info =
			(BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER) + 2 * sizeof(RGBQUAD));

		Info->bmiHeader = { sizeof(BITMAPINFOHEADER),
						   (int)this->width,
						   (int)this->height,
						   1,
						   32,
						   BI_RGB,
						   0,
						   0,
						   0,
						   0,
						   0 };
		Info->bmiColors[0].rgbBlue = 0;
		Info->bmiColors[0].rgbGreen = 0;
		Info->bmiColors[0].rgbRed = 0;
		Info->bmiColors[0].rgbReserved = 0;
		Info->bmiColors[1].rgbBlue = 255;
		Info->bmiColors[1].rgbGreen = 255;
		Info->bmiColors[1].rgbRed = 255;
		Info->bmiColors[1].rgbReserved = 255;

		BYTE* ptr = nullptr;

		static HBITMAP screen_hb =
			CreateDIBSection(screen_dc, Info, DIB_RGB_COLORS, (void**)&ptr, 0, 0);
		static HBITMAP screen_ob = (HBITMAP)SelectObject(screen_dc, screen_hb);
		this->frame_buffer = new BYTE * [height];

		for (unsigned int j = 0; j < this->height; j++) {
			this->frame_buffer[j] = ptr + this->width * 4 * j;
		}
		free(Info);
		this->z_buffer.resize(this->width * this->height);
		std::fill(z_buffer.begin(), z_buffer.end(), 1);
	}

	// clear framebuffer and zbuffer
	void Pipeline::ClearBuffer() {
		memset(this->frame_buffer[0], 0, height * 4 * width); // clear framebuffer
		std::fill(this->z_buffer.begin(), z_buffer.end(), 1); // clear z buffer
	}

	// map x,y to 1 dimension
	int Pipeline::GetIndex(int x, int y) {
		int index = y * this->width + x;
		return (index < 0 || index >(this->height * this->width - 1)) ? 0 : index;
	}

	// model mat
	srmath::Matrix4f Pipeline::GetModelMatrix(const Scene& scene) const {
		return scene.GetModelMatrix();
	}

	// view mat
	srmath::Matrix4f Pipeline::GetViewMatrix(const Camera& camera) const {
		return camera.GetViewMatrix();
	}

	// projection mat (perspective)
	srmath::Matrix4f Pipeline::GetPerspectiveProjectionMatrix(
		const float eye_fov, const float aspect_ratio, const float z_near,
		const float z_far) const {
		srmath::Matrix4f projection;
		projection.SetZero();
		auto fax = 1.0f / (tan(eye_fov * 0.5f));
		// std::cout << fax;
		projection.e[0][0] = fax / aspect_ratio;
		projection.e[1][1] = fax;
		projection.e[2][2] = -(z_far + z_near) / (z_far - z_near);
		projection.e[2][3] = -2 * z_near * z_far / (z_far - z_near);
		projection.e[3][2] = -1;
		return projection;
	}

	// write color to frame_buffer
	void Pipeline::SetPixel(const int x, const int y, const srmath::Color& color) {
		if (x <= this->width - 1 && y <= this->height - 1) {
			BYTE* p = &frame_buffer[y][x * 4];
			*p = color.z;
			*(p + 1) = color.y;
			*(p + 2) = color.x;
		}
	}

	//  draw line
	void Pipeline::DrawLine(int x0, int y0, int x1, int y1,
		const srmath::Color& color) {
		int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
		int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
		int err = (dx > dy ? dx : -dy) / 2, e2;
		for (;;) {
			SetPixel(x0, y0, color);
			if (x0 == x1 && y0 == y1)
				break;
			e2 = err;
			if (e2 > -dx) {
				err -= dy;
				x0 += sx;
			}
			if (e2 < dy) {
				err += dx;
				y0 += sy;
			}
		}
	}

	// draw wireframe
	void Pipeline::RasterizeWireframe(const Triangle& t,
		const srmath::Color& color = { 255, 255, 255 }) {
		DrawLine(t.vertex[0].x, t.vertex[0].y, t.vertex[1].x, t.vertex[1].y, color);
		DrawLine(t.vertex[1].x, t.vertex[1].y, t.vertex[2].x, t.vertex[2].y, color);
		DrawLine(t.vertex[2].x, t.vertex[2].y, t.vertex[0].x, t.vertex[0].y, color);
	}

	// draw
	void Pipeline::Draw(const Camera& camera, int _rendermode) {
		this->rendermode = _rendermode;
		if (object_list.size() == 0)
			return;
		cam = camera;
		view = this->GetViewMatrix(camera);
		projection = GetPerspectiveProjectionMatrix(
			90 * _PI / 180, (float)this->width / this->height, 0.01, 1000);
		for (auto& obj : this->object_list) {
			DrawObject(*obj);
		}
	}

	// draw single object in pipeline
	void Pipeline::DrawObject(Scene& obj) {
		srmath::Matrix4f model = obj.GetModelMatrix();
		srmath::Matrix4f mvp = projection * view * model;
		srmath::Matrix4f mv = view * model;


		srmath::Vector4f view_dir=mv*cam.GetDirection();
		for (auto& i : obj.triangle_list) {

			// vertex translations
			std::vector<srmath::Vector4f> v{ mvp * i->vertex[0], mvp * i->vertex[1],
										  mvp * i->vertex[2] };

			// view space position
			std::vector<srmath::Vector4f> view_space_pos{
			  mv * i->vertex[0], mv * i->vertex[1], mv * i->vertex[2] };

			// perspective division and screen mapping
			for (auto& vert : v) {
				vert.x /= vert.w;
				vert.y /= vert.w;
				vert.z /= vert.w;
				vert.z = 0.5 * (vert.z + 1.0);
				vert.x = 0.5 * this->width * (vert.x + 1.0);
				vert.y = 0.5 * this->height * (vert.y + 1.0);
			}

			// set triangle
			Triangle t;

			// tex mapping correction
			t.SetTexcoord(i->tex_coords[0] / v[0].w, i->tex_coords[1] / v[1].w,
				i->tex_coords[2] / v[2].w);
			v[0].w = 1 / v[0].w;
			v[1].w = 1 / v[1].w;
			v[2].w = 1 / v[2].w;
			t.SetVertex(v[0], v[1], v[2]);
			t.SetNormal(mv * i->normal[0], mv * i->normal[1], mv * i->normal[2]);
			t.SetColor(i->color[0], i->color[1], i->color[2]);

			// back face culling
			srmath::Vector4f normal = srmath::cross(v[1] - v[0], v[2] - v[0]);
			if (srmath::dot(srmath::Vector4f{ 0, 0, 1 }, normal) < 0) {
				continue;
			}
			if (this->rendermode == WIREFRAME) {
				RasterizeWireframe(t);
			}
			else
				DrawTriangle(t, view_space_pos, obj.texture,view_dir);
		}
	}


	// compute barycentric coodinate
	srmath::Vector4f computeBarycentric2D(float x, float y, const sr::Triangle& v) {

		srmath::Vector4f u = srmath::cross(
			srmath::Vector4f(v.vertex[2].x - v.vertex[0].x, v.vertex[1].x - v.vertex[0].x,
				v.vertex[0].x - x),
			srmath::Vector4f(v.vertex[2].y - v.vertex[0].y, v.vertex[1].y - v.vertex[0].y,
				v.vertex[0].y - y));
		if (std::abs(u.z) < 1)
			return srmath::Vector4f(-1, 1, 1);
		return srmath::Vector4f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
	}

	void Pipeline::DrawTriangle(Triangle& v,
		std::vector<srmath::Vector4f>& view_space_pos,
		Texture& tex, srmath::Vector4f view_dir) {

		// calculate bounding box
		float max_x = std::max({ v.vertex[0].x, v.vertex[1].x, v.vertex[2].x });
		float max_y = std::max({ v.vertex[0].y, v.vertex[1].y, v.vertex[2].y });
		float min_x = std::min({ v.vertex[0].x, v.vertex[1].x, v.vertex[2].x });
		float min_y = std::min({ v.vertex[0].y, v.vertex[1].y, v.vertex[2].y });

		for (int i = min_x; i < max_x; i++) {
			for (int j = min_y; j < max_y; j++) {
				// compute barycentric coodinate
				srmath::Vector4f bc_screen = computeBarycentric2D(i, j, v);

				if (bc_screen.x < 0 || bc_screen.y < 0 || bc_screen.z < 0)
					continue;

				float zp = v.vertex[0].z * bc_screen.x + v.vertex[1].z * bc_screen.y +
					v.vertex[2].z * bc_screen.z;

				if (zp < z_buffer[GetIndex(i, j)]) {
					this->z_buffer[GetIndex(i, j)] = zp;

					float w =
						1.0f / (v.vertex[0].w * bc_screen.x + v.vertex[1].w * bc_screen.y +
							v.vertex[2].w * bc_screen.z);

					// interpolations of color/normal/viewpos/texcoord
					auto color_interp{ bc_screen.x * v.color[0] + bc_screen.y * v.color[1] +
									  bc_screen.z * v.color[2] };

					auto normal_interp =
						w * (bc_screen.x * v.normal[0] + bc_screen.y * v.normal[1] +
							bc_screen.z * v.normal[2]);

					// texture mapping correction
					auto view_pos_interp = w * (bc_screen.x * view_space_pos[0] +
						bc_screen.y * view_space_pos[1] +
						bc_screen.z * view_space_pos[2]);

					auto texcoord_interp =
						w * (bc_screen.x * v.tex_coords[0] + bc_screen.y * v.tex_coords[1] +
							bc_screen.z * v.tex_coords[2]);

					fragment_shader_payload pl(color_interp, normal_interp, texcoord_interp,
						&tex);
					
					pl.view_pos = view_pos_interp;
					pl.view_dir = view_dir;
					// translate light dirction
					for (auto& li : light_list) {
						srmath::Vector4f dir = view * li->GetDir();
						srmath::Color    col = li->GetColor();
						DirectLight    a(dir, col);
						pl.light_list.push_back(&a);
					}
					srmath::Color pixel_color;

					// fragment shader to compute pixel color
					switch (rendermode) {
					case TEXTURE:
						pixel_color = sr::texture_shader(pl);
						break;
					case NORMAL:
						pixel_color = sr::normal_shader(pl);
						break;
					case PHONG:
						pixel_color = sr::phong_shader(pl);
						break;
					default:
						break;
					}
					SetPixel(i, j, pixel_color);
				}
			}
		}
	}

} // namespace sr
