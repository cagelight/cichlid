#pragma once

#define GAME_HEADER
#include "common.hh"
#include GAME_PUBLIC

#ifndef GAME_SOURCE
INTERNAL_LEAK_FLAW
#endif

namespace cichlid::game {
	extern std::mt19937_64 rng;
	extern globals_t globals_;
	
	#define CVAR_EXTERNDECL
	#include "^cvar.inl"
}
