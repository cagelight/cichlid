#include "realm.hh"

using namespace cichlid;

game::realm::realm() {
	test_pers.pos = vec3_t {0, 0, 0};
	test_pers.vec = {};
	
	object::cube testc;
	testc.trans.pos = vec3_t {0, 0, 2};
	
	cubes.push_back(testc);
}

game::realm::~realm() {
	
}

void game::realm::frame() {
	vec3_t wam =  test_pers.vec * ci.axial_move;
	test_pers.pos += wam * globals.impulse * 5;
	
	vec3_t axial_x {1, 0, 0};
	vec3_t axial_y {0, 1, 0};
	vec3_t axial_z {0, 0, 1};
	
	quat_t wx {axial_x, ci.axial_turn.x() * globals.impulse};
	quat_t wy {axial_y, ci.axial_turn.y() * globals.impulse};
	quat_t wz {axial_z, ci.axial_turn.z() * globals.impulse};
	
	test_pers.vec = (wx * wy * wz) * test_pers.vec;
}

game::perspective const & game::realm::get_default_perspective() const {
	return test_pers;
}
