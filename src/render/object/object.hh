#pragma once

#include "render/render_common.hh"
#include "game/object/cube.hh"

namespace cichlid::render::object {
	
	struct cube final {
		
		transformation_t trans;
		
		cube() = delete;
		cube(game::object::cube const &);
		cube(cube &&) = default;
		~cube() = default;
		
	};
	
}
