#pragma once

#include "render_common.hh"

namespace cichlid::render::shader {
	
	void init();
	void term();
	
	enum struct type {
		basic
	};
	
	struct basic {
		b::mat4_t<float> m;
	};
	
	void bind(type);
	void upload(basic const &);
	
};
