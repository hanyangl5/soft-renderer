#pragma once
//#include "Camera.h"
#include <iostream>
#include <tchar.h>
#define NOMINMAX
#include <windows.h>
#include <wingdi.h>
#include <string>
namespace sr {
namespace window {

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 800;
static int         screen_keys[512];
static const TCHAR szWindowClass[] = _T("Rasterizer");
static const TCHAR szTitle[] = _T("Rasterizer");
static bool        flag = false;
POINT              cur_pos, last_pos;

class WindowClass {

public:
  HINSTANCE      hInstance;
  HWND           hwnd;
  HDC            screen_dc, hDC;
  WNDCLASSEX     wcex;
  static HBITMAP screen_ob;

  WindowClass();
  friend LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                                  LPARAM lParam);
  void                    UpdateScreen(int fps);
  void                    Show();
  void                    Dispatch();
};

// WndProc 
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  switch (message) {
  // camera rotation
  case WM_MOUSEMOVE: {
    RECT winRect;
    GetClientRect(hWnd, &winRect);
    ClientToScreen(hWnd, (LPPOINT)&winRect.left);
    ClientToScreen(hWnd, (LPPOINT)&winRect.right);
    ClipCursor(&winRect);

    if (screen_keys[VK_SHIFT]) {
      if (!flag) {
        GetCursorPos(&cur_pos);
        last_pos = cur_pos;
        flag = true;
      }
      last_pos = cur_pos;
      GetCursorPos(&cur_pos);
      // flag = false;
    } else
      flag = false;
    break;
  }
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  case WM_KEYDOWN:
    screen_keys[wParam & 511] = 1;
    break;
  case WM_KEYUP:
    screen_keys[wParam & 511] = 0;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
    break;
  }

  return 0;
}

// ctor
WindowClass::WindowClass() {
  wcex.cbSize = sizeof(WNDCLASSEX);
  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
  wcex.lpszMenuName = NULL;
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

  if (!RegisterClassEx(&wcex)) {
    std::cout << "failed to register class\n";
    exit(1);
  }

  hwnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED | WS_SYSMENU,
                      CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH, WINDOW_HEIGHT,
                      NULL, NULL, hInstance, NULL);

  if (!hwnd) {
    std::cout << "failed to create window\n";
    exit(1);
  }

  hDC = GetDC(hwnd);
  screen_dc = CreateCompatibleDC(hDC);
  ReleaseDC(hwnd, hDC);
  memset(screen_keys, 0, sizeof(int) * 512);
}

// update screen buffer and display fps
void WindowClass::UpdateScreen(int fps) {
  HDC hDC = GetDC(hwnd);
  BitBlt(hDC, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, screen_dc, 0, 0, SRCCOPY);
  LPCSTR fps_text = (std::to_string(fps).c_str()); // convert int 2 lpcstr
  SetWindowTextA(hwnd, fps_text);
  ReleaseDC(hwnd, hDC);
}

// show
void WindowClass::Show() { ShowWindow(hwnd, SW_NORMAL); }

// dispatch message
void WindowClass::Dispatch() {
  MSG msg;
  while (1) {
    if (!PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
      break;
    if (!GetMessage(&msg, NULL, 0, 0))
      break;
    DispatchMessage(&msg);
  }
}

} // namespace window

} // namespace sr