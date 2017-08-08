#pragma once

#include "render_common.hh"

namespace cichlid::render::mesh {
	
	void init();
	void term();
	
	enum struct primitive {
		cube
	};
	
	void bind(primitive);
	void draw();
};
