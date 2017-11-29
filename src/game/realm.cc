#include "realm.hh"
#include "game_private.hh"

ci::game::realm::realm() {
	//pers.pos = vec3_t {-1.7, 1.5, 0.7};
	pers.pos = vec3_t {-2, -1, -4};
	
	test_cube tc;
	
	std::uniform_real_distribution<double> posr {-10, 10};
	std::uniform_real_distribution<double> rater {0.25, 1.5};
	std::uniform_real_distribution<double> colorr {0, 1};
	std::uniform_real_distribution<double> rotr {0, pi_2 * 4};
	std::uniform_real_distribution<double> scaler {0.5, 1.5};
	
	for (int i = 0; i < 100; i++) {
		tc.tA.pos = {posr(rng), posr(rng), posr(rng)};
		vec3_t rotv {colorr(rng), colorr(rng), colorr(rng)};
		rotv.normalize();
		tc.tA.rot = {rotv, rotr(rng)};
		tc.tA.scale = {scaler(rng)};
		tc.tB.pos = {posr(rng), posr(rng), posr(rng)};
		rotv = {colorr(rng), colorr(rng), colorr(rng)};
		rotv.normalize();
		tc.tB.rot = {rotv, rotr(rng)};
		tc.tB.scale = {scaler(rng)};
		tc.crate = rater(rng);
		tc.cA = {colorr(rng), colorr(rng), colorr(rng)};
		tc.cB = {colorr(rng), colorr(rng), colorr(rng)};
		
		test_cubes.push_back(tc);
	}
	
	/*
	object::cube testc;
	testc.trans.pos = vec3_t {0, 0, 0};
	testc.color = {0, 0.5, 1};
	
	cubes.push_back(testc);
	*/
	
	//pers.vec = quat_t::look_at(pers.pos, testc.trans.pos, {0, 1, 0});
}

void ci::game::realm::frame() {
	vec3_t wamd = pers.vec * conint.direct.axial_move;
	vec3_t wami = pers.vec * conint.impulsed.axial_move;
	pers.pos += wamd * 5;
	pers.pos += wami * globals.impulse * 5;
	
	bool gravity = true;
	vec3_t gravity_vector = g_gravity_vector;
	gravity_vector.normalize();
	
	vec3_t axial_x {1, 0, 0};
	vec3_t axial_y;
	vec3_t axial_z {0, 0, 1};
	
	if (gravity) {
		axial_y = -gravity_vector * pers.vec.conjugate();
	} else {
		axial_y = {0, 1, 0};
	}
	
	quat_t wx {axial_x, conint.direct.axial_turn.x() + (conint.impulsed.axial_turn.x() * globals.impulse)};
	quat_t wy {axial_y, conint.direct.axial_turn.y() + (conint.impulsed.axial_turn.y() * globals.impulse)};
	quat_t wz {axial_z, conint.direct.axial_turn.z() + (conint.impulsed.axial_turn.z() * globals.impulse)};
	
	pers.vec = (wx * wy * wz) * pers.vec;
	if (gravity) {
		pers.vec *= pers.vec.roll_up(-gravity_vector, 1);
		pers.vec *= pers.vec.range_limit_up(-gravity_vector, 0.99999999999L * pi_h, 1);
	}
	
	for (test_cube & tc : test_cubes) {
		double lerp = (sin(globals.msec * (tc.crate / 1000)) + 1) / 2;
		tc.tC = transformation_t::lerp(tc.tA, tc.tB, lerp);
		tc.cC = b::lerp(tc.cA, tc.cB, lerp);
	}
	
	//if (g_log_player_transform) scilogi << pers.pos << pers.vec;
}
