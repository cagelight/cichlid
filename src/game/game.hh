#pragma once

#include "common.hh"
#include "unit.hh"

namespace cichlid::game {
	
	struct realm;
	
	void init();
	void term();
	
	void frame(as::time::span const &);
	
	struct globals_t {
		double impulse = 0;
		uint_fast64_t msec = 0;
	};
	
	extern globals_t const & globals;
	
	// CONTROL
	
	struct control_interface {
		vec3_t axial_move;
		vec3_t axial_turn;
	};
	
	extern control_interface ci; // reset at the end of each game frame
	
	// RENDER
	
	realm * get_root_realm();
	
}
