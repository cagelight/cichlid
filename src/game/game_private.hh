#pragma once

#include "game.hh"
#include <random>

namespace ci::game {
	extern std::mt19937_64 rng;
	extern globals_t globals_;
	
	#define CVAR_EXTERNDECL
	#include "xcvar.inl"
}
