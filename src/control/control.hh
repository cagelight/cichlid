#pragma once

#include "common/common.hh"

namespace ci::control {
	
	void init();
	void term() noexcept;
	
	void frame();
	b::vec2_t<size_t> context_size();
	
}
