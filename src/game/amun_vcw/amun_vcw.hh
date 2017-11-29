#pragma once

#include "../game.hh"

namespace ci::game::amun {
	
	struct voxel {
		// CORE
		uint16_t id : 16;
		// LIGHTING
		uint16_t sun_lock : 1; // lock all following values to the world sun's values
		uint16_t luminosity : 5; // brightness, also directly correlates with voxel travel (10 -> 10 brightness and travels 10 voxels)
		uint16_t vibrance : 2; // 0 -> white, 3 -> full color
		uint16_t hue : 8; // 0 -> red, 255 -> 1 step before red
		// MISC
		uint16_t damage : 4;
		uint16_t rotation : 5;
		uint16_t chisel : 7;
	};
	
	static_assert( sizeof(voxel) == 6 );
	
	struct chunk {
		std::vector<voxel> data;
	};
	
	struct world {
		typedef b::vec3_t<uint_fast64_t> chunk_coord_t;
		std::unordered_map<chunk_coord_t, std::unique_ptr<chunk>> chunks;
		
		world();
		world(world const &) = delete;
		
		void focus(pos_t const &);
	};
	
}
