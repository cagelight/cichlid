#pragma once

#include "render_common.hh"

#include "game/game.hh"
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
