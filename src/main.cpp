
#include <cstdint>
#include <stdexcept>

#include "Pipeline.h"
#include "SDL_window.h"

const uint32_t width{1280}, height{800};

int main(int argc, char **argv) {

    Window main_window(width, height);

    main_window.Run();
}
