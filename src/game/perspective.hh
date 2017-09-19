#pragma once

#define GAME_HEADER
#include "common.hh"
#include GAME_PUBLIC

namespace cichlid::game {
	
	struct perspective {
		vec3_t pos {};
		quat_t vec;
	};
	
}
