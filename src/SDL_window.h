#pragma once

#include <cstdint>
#include <iostream>
#include <memory>

#ifndef SDL_MAIN_HANDLED
#define SDL_MAIN_HANDLED
#include <SDL.h>
#endif

#include <glm/glm.hpp>

#include "Camera.h"
#include "Pipeline.h"

//#include <SDL2/SDL_image.h>
class Window {
  public:
    Window(uint32_t w, uint32_t h) : width(w), height(h) {

        //init sdl
        SDL_Init(SDL_INIT_VIDEO);
        // create window
        SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
        if (window == nullptr) {
            SDL_Log("failed to create window: %s", SDL_GetError());
            exit(-1);
        }
        // clear color
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    ~Window() {
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        std::cout << "window destroyed\n";
    }
    Window(const Window &) = default;
    Window(Window &&) = delete;
    Window &operator=(const Window &) = default;
    Window &operator=(Window &&) = delete;
    void Run() {

        auto pipeline = std::make_shared<Pipeline>(width, height);
        camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 10.0f));
        pipeline->InitAsset();

        int shift = 24;
        int rmask = 0xff000000 >> shift;
        int gmask = 0x00ff0000 >> shift;
        int bmask = 0x0000ff00 >> shift;
        int amask = 0x000000ff >> shift;

        while (true) {
            if (SDL_PollEvent(&event) && event.type == SDL_QUIT) {
                break;
            }

            Tick();

            HandleInput(delta_time);
            pipeline->Update(camera, delta_time);
            pipeline->Render();

            surface = SDL_CreateRGBSurfaceFrom(pipeline->GetColorBuffer(), width, height, 24, 3 * width, rmask, gmask,
                                               bmask, amask);
            SDL_SetWindowTitle(window, window_title.c_str());
            if (surface == nullptr) {
                SDL_Log("failed to create surface: %s", SDL_GetError());
                continue;
            }
            texture = SDL_CreateTextureFromSurface(renderer, surface);
            if (texture == nullptr) {
                SDL_Log("failed to create texture: %s", SDL_GetError());
                continue;
            }

            SDL_RenderCopy(renderer, texture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
            pipeline->Clear();
            SDL_DestroyTexture(texture); // destroy texture to prevent memory leak
            SDL_FreeSurface(surface);
        }
    }

  private:
    void Tick() {

        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        delta_time = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());
        window_title = std::to_string(delta_time * 1000);
        //std::cout << delta_time << "\n";
    }
    void HandleInput(float deltaTime) {

        const Uint8 *keystate = SDL_GetKeyboardState(NULL);
        SDL_GetMouseState(&cursor_x, &cursor_y);
        //continuous-response keys
        if (keystate[SDL_SCANCODE_A] != 0u) {
            camera->ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
            //std::cout << "left key pressed\n";
        }
        if (keystate[SDL_SCANCODE_D] != 0u) {
            camera->ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
            //std::cout << "right key pressed\n";
        }
        if (keystate[SDL_SCANCODE_W] != 0u) {
            camera->ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
            //std::cout << "up key pressed\n";
        }
        if (keystate[SDL_SCANCODE_S] != 0u) {
            camera->ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
            //std::cout << "down key pressed\n";
        }

        //if (keystate[SDL_SCANCODE_SPACE]) {
        //first_mouse = true;
        HandleMouseMove(cursor_x, cursor_y);
        //}

        //if (keystate[SDL_SCANCODE_SPACE] && keystate[SDL_SCANCODE_UP]) {
        //first_mouse = true;
        //}
    }

    void HandleMouseMove(uint32_t xpos, uint32_t ypos) {

        const Uint8 *keystate = SDL_GetKeyboardState(nullptr);

        if (first_mouse) {
            lastX = xpos;
            lastY = ypos;
            first_mouse = false;
        }

        int xoffset{static_cast<int>(xpos - lastX)};
        int yoffset{static_cast<int>(lastY - ypos)};

        lastX = xpos;
        lastY = ypos;

        if (keystate[SDL_SCANCODE_SPACE] != 0u) {
            camera->ProcessMouseMovement(xoffset, yoffset);
        }
    }

    // fps calculating
    uint32_t LAST{0}, NOW{0};
    float delta_time{};
    bool first_mouse{true};
    std::string window_title{};
    // input handling
    uint32_t lastX{static_cast<uint32_t>(width / 2.0f)};
    uint32_t lastY{static_cast<uint32_t>(height / 2.0f)};
    int cursor_x{}, cursor_y{};

    // camera
    std::shared_ptr<Camera> camera{};

    // window info
    uint32_t width{}, height{};
    SDL_Event event{};
    SDL_Renderer *renderer{};
    SDL_Window *window{};
    SDL_Texture *texture{};
    SDL_Surface *surface{};
};