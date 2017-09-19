#pragma once

#define RENDER_HEADER
#include "common.hh"
#include RENDER_PUBLIC

#include "protoframe.hh"
#include "shader.hh"
#include "mesh.hh"
#include "font.hh"

namespace cichlid::render {
	
	struct command_sequence {
		
		std::vector<shader::basic> basic_seq;
		
		struct {
			mat4_t m_x, m_y, m_z;
		} origin_lines;
		
		bool console_showing;
		bool console_showing_autocomplete;
		std::vector<std::string> console_autocomplete;
		std::string console_string;
		
		command_sequence(protoframe const &);
		
		void execute();
	};
	
}
