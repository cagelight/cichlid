#pragma once

#define RENDER_HEADER
#include "common.hh"
#include RENDER_PUBLIC

#ifndef RENDER_SOURCE
INTERNAL_LEAK_FLAW
#endif

#include "bridge/gl_common.hh"
#include "bridge/text.hh"

namespace cichlid::render {
	extern size_t context_width, context_height;
	
	extern std::mutex cvar_mutex;
	
	static constexpr size_t console_text_size = 10;
	void bind_console_text_atlas();
	bridge::text::string console_text_generate(std::string const & str);
	
	#define CVAR_EXTERNDECL
	#include "^cvar.inl"
}
