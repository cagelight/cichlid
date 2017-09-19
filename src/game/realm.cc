#define GAME_SOURCE
#include "common.hh"
#include GAME_PRIVATE

#include "realm.hh"

using namespace cichlid;

game::realm::realm() {
	//test_pers.pos = vec3_t {-1.7, 1.5, 0.7};
	test_pers.pos = vec3_t {-2, -1, -4};
	
	object::cube testc;
	testc.trans.pos = vec3_t {0, 0, 0};
	testc.color = {0, 0.5, 1};
	
	cubes.push_back(testc);
	
	test_pers.vec = quat_t::look_at(test_pers.pos, testc.trans.pos, {0, 1, 0});
}

game::realm::~realm() {
	
}

void game::realm::frame() {
	vec3_t wamd = test_pers.vec * ci.direct.axial_move;
	vec3_t wami = test_pers.vec * ci.impulsed.axial_move;
	test_pers.pos += wamd * 5;
	test_pers.pos += wami * globals.impulse * 5;
	
	bool gravity = true;
	vec3_t gravity_vector = g_gravity_vector;
	gravity_vector.normalize();
	
	vec3_t axial_x {1, 0, 0};
	vec3_t axial_y;
	vec3_t axial_z {0, 0, 1};
	
	if (gravity) {
		axial_y = -gravity_vector * test_pers.vec.conjugate();
	} else {
		axial_y = {0, 1, 0};
	}
	
	quat_t wx {axial_x, ci.direct.axial_turn.x() + (ci.impulsed.axial_turn.x() * globals.impulse)};
	quat_t wy {axial_y, ci.direct.axial_turn.y() + (ci.impulsed.axial_turn.y() * globals.impulse)};
	quat_t wz {axial_z, ci.direct.axial_turn.z() + (ci.impulsed.axial_turn.z() * globals.impulse)};
	
	test_pers.vec = (wx * wy * wz) * test_pers.vec;
	if (gravity) {
		test_pers.vec *= test_pers.vec.roll_up(-gravity_vector, 1);
		test_pers.vec *= test_pers.vec.range_limit_up(-gravity_vector, 0.99999999999L * pi_h, 1);
	}
	
	if (g_log_player_transform) scilogi << test_pers.pos << test_pers.vec;
}

game::perspective const & game::realm::get_default_perspective() const {
	return test_pers;
}
