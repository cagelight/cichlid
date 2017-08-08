#pragma once

#include "protoframe.hh"
#include "shader.hh"
#include "mesh.hh"

namespace cichlid::render {
	
	struct command_sequence {
		
		std::vector<shader::basic> basic_seq;
		
		command_sequence(protoframe const &);
		
		void execute();
	};
	
}
