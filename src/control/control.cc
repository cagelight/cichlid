#include "game/game.hh"
#include "bridge/sdl2_common.hh"
#include "control.hh"

SDL_Window * ci::sdl2::win = nullptr;
SDL_Surface * ci::sdl2::surface = nullptr;

size_t ci::sdl2::context_width = 1600;
size_t ci::sdl2::context_height = 900;

static Uint8 const * keystate;
static bool should_grab = true;
static bool is_grabbing = false;

void ci::control::init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		scilogve << "failed to initialize SDL2";
		throwe(startup);
	}
	sdl2::win = SDL_CreateWindow(u8"Project Cichlid 🦕", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, sdl2::context_width, sdl2::context_height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!sdl2::win) {
		scilogve << "failed to create window";
		throwe(startup);
	}
	sdl2::surface = SDL_GetWindowSurface(sdl2::win);
	SDL_FillRect(sdl2::surface, nullptr, SDL_MapRGB(sdl2::surface->format, 0x00, 0x00, 0x00));
	SDL_UpdateWindowSurface(sdl2::win);
	
	SDL_GL_LoadLibrary(nullptr);
	
	keystate = SDL_GetKeyboardState(NULL);
}

void ci::control::term() noexcept {
	if (sdl2::win) SDL_DestroyWindow(sdl2::win);
	SDL_Quit();
	
	sdl2::surface = nullptr;
	sdl2::win = nullptr;
}

static void handle_text(SDL_TextInputEvent const & tev) {
	ci::game::conint.ascii_input += tev.text;
}

static void handle_key(SDL_KeyboardEvent const & kev, bool down) {
	if (down) {
		switch (kev.keysym.scancode) {
			case SDL_SCANCODE_LALT:
				should_grab = !should_grab;
				break;
			case SDL_SCANCODE_GRAVE:
				ci::game::conint.trigger.console_toggle = true;
				break;
			case SDL_SCANCODE_RETURN:
				ci::game::conint.trigger.console_enter = true;
				break;
			case SDL_SCANCODE_BACKSPACE:
				ci::game::conint.trigger.text_backspace = true;
				break;
			case SDL_SCANCODE_TAB:
				ci::game::conint.trigger.console_autocomplete = true;
				break;
			case SDL_SCANCODE_UP:
				ci::game::conint.trigger.console_history_up = true;
				break;
			case SDL_SCANCODE_DOWN:
				ci::game::conint.trigger.console_history_down = true;
				break;
			default:
				break;
		}
	}
}

static void handle_mouse(SDL_MouseMotionEvent const & mev) {
	ci::game::conint.direct.axial_turn.x() -= mev.yrel * 0.0025;
	ci::game::conint.direct.axial_turn.y() -= mev.xrel * 0.0025;
}

void ci::control::frame() {
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
						sdl2::context_width = e.window.data1;
						sdl2::context_height = e.window.data2;
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
	
	if (keystate[SDL_SCANCODE_W]) ci::game::conint.impulsed.axial_move.z() += 1;
	if (keystate[SDL_SCANCODE_S]) ci::game::conint.impulsed.axial_move.z() -= 1;
	if (keystate[SDL_SCANCODE_D]) ci::game::conint.impulsed.axial_move.x() += 1;
	if (keystate[SDL_SCANCODE_A]) ci::game::conint.impulsed.axial_move.x() -= 1;
	if (keystate[SDL_SCANCODE_SPACE]) ci::game::conint.impulsed.axial_move.y() += 1;
	if (keystate[SDL_SCANCODE_C]) ci::game::conint.impulsed.axial_move.y() -= 1;
	if (keystate[SDL_SCANCODE_E]) ci::game::conint.impulsed.axial_turn.z() += 1;
	if (keystate[SDL_SCANCODE_Q]) ci::game::conint.impulsed.axial_turn.z() -= 1;
	
	if (should_grab && !is_grabbing) {
		SDL_SetRelativeMouseMode(SDL_TRUE);
		is_grabbing = true;
	} else if (!should_grab && is_grabbing) {
		SDL_SetRelativeMouseMode(SDL_FALSE);
		is_grabbing = false;
	}
}

b::vec2_t<size_t> ci::control::context_size() {
	return {sdl2::context_width, sdl2::context_height};
}
