#pragma once

#define GAME_HEADER
#include "common.hh"
#include GAME_PUBLIC

namespace cichlid::game::object {
	
	struct cube final {
		
		transformation_t trans;
		b::vec3_t<double> color;
		
		cube() = default;
		~cube() = default;
		
	};
	
}
