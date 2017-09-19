#pragma once

#define RENDER_HEADER
#include "common.hh"
#include RENDER_PUBLIC

#include "game/object/cube.hh"

namespace cichlid::render::object {
	
	struct cube final {
		
		transformation_t trans;
		b::vec3_t<float> color;
		
		cube() = delete;
		cube(game::object::cube const & gc) : trans(gc.trans), color(gc.color) {}
		cube(cube &&) = default;
		~cube() = default;
		
	};
	
}
