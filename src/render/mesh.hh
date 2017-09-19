#pragma once

#define RENDER_HEADER
#include "common.hh"
#include RENDER_PUBLIC

namespace cichlid::render::mesh {
	
	void init();
	void term();
	
	enum struct primitive {
		line_x, // unit line, X positive
		line_y, // unit line, X positive
		line_z, // unit line, X positive
		uqzn, // unit quad, facing Z negative
		cube
	};
	
	void bind(primitive);
	void draw();
};
