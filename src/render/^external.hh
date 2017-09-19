#pragma once

#define RENDER_HEADER
#include "common.hh"

namespace cichlid::render {
	
	void init();
	void term();
	
	void preframe();
	void frame();
	
}
