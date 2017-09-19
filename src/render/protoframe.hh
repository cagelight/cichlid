#pragma once

#define RENDER_HEADER
#include "common.hh"
#include RENDER_PUBLIC

#include GAME_PUBLIC
#include "game/realm.hh"

#include "object/object.hh"

#include <vector>

namespace cichlid::render {
	
	struct protoframe {
		
		protoframe(game::realm const &, game::perspective const &);
		
		game::perspective pers;
		std::vector<render::object::cube> cubes;
	};
	
};
