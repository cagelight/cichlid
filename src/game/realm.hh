#pragma once

#define GAME_HEADER
#include "common.hh"
#include GAME_PUBLIC

#include "perspective.hh"
#include "object/cube.hh"

#include "bridge/text.hh"

#include <list>

namespace cichlid::game {
	
	struct realm {
		
		perspective test_pers {};
		std::list<object::cube> cubes;
		bridge::text::string test_string;
		
		realm();
		~realm();
		
		void frame();
		
		perspective const & get_default_perspective() const;
		
	};
	
}
