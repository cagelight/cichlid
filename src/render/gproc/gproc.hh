#pragma once

#include "../render_private.hh"

namespace ci::render::gproc {
	
	void init();
	void term() noexcept;
	
	void resize_buffers();
	
	void generate_proto();
	void generate_cmdseq();
	void draw();
	
}
