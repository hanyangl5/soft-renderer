
#include "Pipeline.h"
#include <cstdint>
#include "SDL_window.h"
const uint32_t width{ 1280 }, height{ 800 };

int main(int argc, unsigned char* argv[]) {

	Window main_window(width, height);
	main_window.Run();

}
