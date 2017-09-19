#define CONTROL_SOURCE
#include "common.hh"
#include CONTROL_PRIVATE

#include "bridge/sdl2_common.hh"

#include GAME_PUBLIC

#include <cstring>

using namespace cichlid;

SDL_Window * sdl::win = nullptr;
SDL_Surface * sdl::surface = nullptr;
size_t sdl::context_width = 1600;
size_t sdl::context_height = 900;

static Uint8 const * keystate;

void control::init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cilogve("failed to initialize SDL2");
		throwe(startup);
	}
	sdl::win = SDL_CreateWindow(u8"Project Cichlid 🦕", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sdl::context_width, sdl::context_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!sdl::win) {
		cilogve("failed to create window");
		throwe(startup);
	}
	sdl::surface = SDL_GetWindowSurface(sdl::win);
	SDL_FillRect(sdl::surface, nullptr, SDL_MapRGB(sdl::surface->format, 0x00, 0x00, 0x00));
	SDL_UpdateWindowSurface(sdl::win);
	
	SDL_GL_LoadLibrary(nullptr);
	
	keystate = SDL_GetKeyboardState(NULL);
}

void control::term() {
	if (sdl::win) SDL_DestroyWindow(sdl::win);
	SDL_Quit();
	
	sdl::surface = nullptr;
	sdl::win = nullptr;
}

static bool should_grab = true;
static bool is_grabbing = false;

static void handle_text(SDL_TextInputEvent const & tev) {
	game::ci.ascii_input += tev.text;
}

static void handle_key(SDL_KeyboardEvent const & kev, bool down) {
	switch (kev.keysym.scancode) {
		case SDL_SCANCODE_LALT:
			if (down) should_grab = !should_grab;
			break;
		case SDL_SCANCODE_GRAVE:
			if (down) game::ci.trigger.console_toggle = true;
			break;
		case SDL_SCANCODE_RETURN:
			if (down) game::ci.trigger.console_enter = true;
			break;
		case SDL_SCANCODE_BACKSPACE:
			if (down) game::ci.trigger.text_backspace = true;
			break;
		case SDL_SCANCODE_TAB:
			if (down) game::ci.trigger.console_autocomplete = true;
			break;
		case SDL_SCANCODE_UP:
			if (down) game::ci.trigger.console_history_up = true;
			break;
		case SDL_SCANCODE_DOWN:
			if (down) game::ci.trigger.console_history_down = true;
			break;
		default:
			break;
	}
}

static void handle_mouse(SDL_MouseMotionEvent const & mev) {
	game::ci.direct.axial_turn.x() -= mev.yrel * 0.0025;
	game::ci.direct.axial_turn.y() -= mev.xrel * 0.0025;
}

void control::frame() {
	SDL_Event e;
	
	while( SDL_PollEvent(&e) != 0 ) {
		switch (e.type) {
			case SDL_QUIT:
				com::run_sem.store(false);
				break;
			case SDL_KEYDOWN:
				handle_key(e.key, true);
				break;
			case SDL_KEYUP:
				handle_key(e.key, false);
				break;
			case SDL_TEXTINPUT:
				handle_text(e.text);
				break;
			case SDL_MOUSEMOTION:
				if (is_grabbing) handle_mouse(e.motion);
				break;
			case SDL_WINDOWEVENT:
				switch (e.window.event) {
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						sdl::context_width = e.window.data1;
						sdl::context_height = e.window.data2;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
	
	if (keystate[SDL_SCANCODE_W]) game::ci.impulsed.axial_move.z() += 1;
	if (keystate[SDL_SCANCODE_S]) game::ci.impulsed.axial_move.z() -= 1;
	if (keystate[SDL_SCANCODE_D]) game::ci.impulsed.axial_move.x() += 1;
	if (keystate[SDL_SCANCODE_A]) game::ci.impulsed.axial_move.x() -= 1;
	if (keystate[SDL_SCANCODE_SPACE]) game::ci.impulsed.axial_move.y() += 1;
	if (keystate[SDL_SCANCODE_C]) game::ci.impulsed.axial_move.y() -= 1;
	if (keystate[SDL_SCANCODE_E]) game::ci.impulsed.axial_turn.z() += 1;
	if (keystate[SDL_SCANCODE_Q]) game::ci.impulsed.axial_turn.z() -= 1;
	
	if (should_grab && !is_grabbing) {
		SDL_SetRelativeMouseMode(SDL_TRUE);
		is_grabbing = true;
	} else if (!should_grab && is_grabbing) {
		SDL_SetRelativeMouseMode(SDL_FALSE);
		is_grabbing = false;
	}
}
