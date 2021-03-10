
#include "WindowClass.h"
#include "Scene.h"
#include "Camera.h"
#include "Pipeline.h"


int main() {

  sr::window::WindowClass windowclass;
  sr::Pipeline pipeline(windowclass.screen_dc, sr::window::WINDOW_WIDTH,
                        sr::window::WINDOW_HEIGHT);
  windowclass.Show();

  // init models
  sr::Camera camera(srmath::Vector4f(2, 2, 5),
                    srmath::Vector4f(0, 0, 0)); // init camera

  //sr::Scene plane("../resources/model/plane.obj");
  //plane.BindTexture(sr::Texture("../resources/textures/container.jpg"));
  //plane.SetScale(2);
  //pipeline.AddToPipeline(plane);

  sr::Scene cube_model("../resources/model/Crate/crate_tri.obj");
  cube_model.BindTexture(sr::Texture("../resources/textures/tex.jpg"));
  cube_model.SetTranslate(srmath::Vector4f(0, 1, 1));
  cube_model.SetScale(0.3);
  pipeline.AddToPipeline(cube_model);

  sr::Scene cube_model2("../resources/model/Crate/crate_tri.obj");
  cube_model2.SetTranslate(srmath::Vector4f(3, 1, 1));
  cube_model2.BindTexture(sr::Texture("../resources/model/Crate/crate_1.jpg"));
  cube_model2.SetScale(0.3);
  pipeline.AddToPipeline(cube_model2);

  sr::Scene rock("../resources/model/rock/rock.obj");
  rock.BindTexture("../resources/model/rock/rock.png");
  rock.SetTranslate(srmath::Vector4f(0, 1, -3));
  rock.SetScale(0.5);
  pipeline.AddToPipeline(rock);

  sr::DirectLight light1(srmath::Vector4f(-1, -1, -0.5, 0),
                         srmath::Color(255, 255, 255));
  pipeline.AddLight(light1);

  clock_t start{}, end{};
  float   fps{0.0f};

  while (sr::window::screen_keys[VK_ESCAPE] == 0) {
    start = clock();
    fps = 1 / ((double)(start - end) / CLOCKS_PER_SEC);
    end = start;

    pipeline.ClearBuffer();
    windowclass.Dispatch();
    ProcessKeyInput(camera);
    camera.CameraRotate((int)(sr::window::last_pos.x - sr::window::cur_pos.x),
                        (int)(sr::window::last_pos.y - sr::window::cur_pos.y));
    cube_model.SetRotate(srmath::Vector4f(0, 1, 0), 0.01);
    cube_model2.SetRotate(srmath::Vector4f(1, 1, 0), 0.02);
    pipeline.Draw(camera, sr::PHONG);
    //std::cout << fps<<"\n";
    windowclass.UpdateScreen(fps);
  }
}
