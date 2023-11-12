#include <tinydream.h>

#include <iostream>
#include <string>

#include "tinydream.hpp"

#if defined (_WIN32) || defined(_WIN64) || defined (_MSC_VER)
#include <Windows.h>
#define TD_WIN
#else
/* Assume POSIX compatible */
#include <unistd.h>
#endif
void logCallback(const char* zLogMsg, int msgLen, [[maybe_unused]] void* pCookie)
{
#ifdef TD_WIN
	WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), static_cast<LPCVOID>(zLogMsg), static_cast<DWORD>(msgLen), 0, 0);
#else
	write(STDOUT_FILENO, static_cast<const void*>(zLogMsg), static_cast<size_t>(msgLen));
#endif /* __WINT__ */
}

#include "tinydream.hpp"

using namespace std;

int generate_image(int upscale, int  step, int seed, const char* positive_prompt, const char* negative_prompt, const char* dst, const char* assets_dir)
{
	if (seed == 0) {
		seed = (unsigned)time(NULL);
	}

	tinyDream td; // stack allocated tinyDream object
    std::string positive_prompt_str = positive_prompt;
    std::string negative_prompt_str = negative_prompt;
    std::string dst_str = dst;
	std::string assets_dir_str = assets_dir;

	// Display the library current inference engine, version number, and copyright notice
	std::cout << tinyDream::about() << std::endl;
	
	// Register a log handler callback responsible of 
	// consuming log messages generated during inference.
	td.setLogCallback(logCallback, nullptr);
	
	// Set the assets path if the pre-trained models
	// are not extracted on the same directory as your executable
	// The Tiny-Dream assets can be downloaded from: https://pixlab.io/tiny-dream#downloads
	td.setAssetsPath(assets_dir_str);
	
	// Set the directory where you want
	// the generated images to be stored
	td.setImageOutputPath(dst_str);
	
	/*
	* Finally, run Stable Diffusion in inference
	* 
	* The supplied log consumer callback registered previously should shortly receive
	* all generated log messages (including errors if any) during inference.
	* 
	* Refer to the official documentation at: https://pixlab.io/tiny-dream#tiny-dream-method
	* for the expected parameters the tinyDream::dream() method takes.
	*/
	td.dream(
		positive_prompt_str,
		negative_prompt_str,
		dst_str, 
		upscale, /* Set to 0=false if you want 512x512 pixels output instead of 2048x2048 output */
		seed,
		step
	);

	// You do not need to display the generated image path manually each time via std::cout
	// as the supplied log callback should have already done that.
	std::cout << "Output Image location: " << dst << std::endl; // uncomment this if too intrusive
	
	return 0;
}
