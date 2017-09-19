#pragma once

#include "gl_common.hh"
#include <SDL2/SDL.h>

namespace cichlid::sdl {
	
	extern SDL_Window * win;
	extern SDL_Surface * surface;
	extern size_t context_width, context_height;
	
}
