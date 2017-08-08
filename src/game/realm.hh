#pragma once

#include "game_common.hh"
#include "perspective.hh"

#include "object/cube.hh"

#include <list>

namespace cichlid::game {
	
	struct realm {
		
		perspective test_pers;
		std::list<object::cube> cubes;
		
		realm();
		~realm();
		
		void frame();
		
		perspective const & get_default_perspective() const;
		
	};
	
}
