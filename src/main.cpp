
#include "Pipeline.h"
#include <cstdint>
#include "SDL_window.h"
#include <stdexcept>
const uint32_t width{ 1280 }, height{ 800 };

int main(int argc, unsigned char* argv[]) {

	Window main_window(width, height);
	try
	{
		main_window.Run();
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}


}
