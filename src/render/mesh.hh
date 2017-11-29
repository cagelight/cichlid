#pragma once

#include "render.hh"

namespace ci::render::mesh {
	
	void init();
	void term();
	
	enum struct primitive {
		line_x, // unit line, X positive
		line_y, // unit line, X positive
		line_z, // unit line, X positive
		uqzn, // unit quad, facing Z negative
		fqzn, // fullscreen quad, facing Z negative
		cube
	};
	
	void bind(primitive);
	void draw();
};
