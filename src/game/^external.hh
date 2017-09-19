#pragma once

#define GAME_HEADER
#include "common.hh"

#include <random>

namespace cichlid::game {
	
	struct realm;
	
	void init();
	void term();
	
	void frame(as::time::span const &);
	
	// CONTROL
	
	struct control_interface {
		struct analog_controls {
			vec3_t axial_move {};
			vec3_t axial_turn {};
		};
		struct triggered_controls {
			bool text_backspace = false;
			bool console_toggle = false;
			bool console_enter = false;
			bool console_autocomplete = false;
			bool console_history_up = false;
			bool console_history_down = false;
		};
		std::string ascii_input;
		analog_controls direct {};
		analog_controls impulsed {};
		triggered_controls trigger {};
	};
	
	extern control_interface ci; // reset at the end of each game frame
	
	struct globals_t {
		double impulse = 0;
		uint_fast64_t msec = 0;
		bool in_console = false;
		std::string console_text;
		std::string console_cache_text;
		bool show_autocomplete = false;
		std::vector<std::string> console_autocomplete_options;
		size_t history_index = 0;
	};
	
	extern globals_t const & globals;
	
	// RENDER
	
	realm * get_root_realm();
	
}
