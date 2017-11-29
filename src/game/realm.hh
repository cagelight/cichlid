#include "game.hh"

#include "amun_vcw/amun_vcw.hh"
#include "perspective.hh"

namespace ci::game {
	
	struct test_cube {
		transformation_t tA, tB, tC;
		double crate = 1;
		vec3_t cA, cB, cC;
	};

	struct realm {
		perspective pers;
		
		std::vector<test_cube> test_cubes;
		amun::world world;
		
		realm();
		
		void frame();
	};
	
};
