#pragma once

#include "common/common.hh"

namespace ci::render {
	
	void init();
	void term() noexcept;
	
	void preframe();
	void frame();
	
}
