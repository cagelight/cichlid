#include "game.hh"
#include "realm.hh"

using namespace cichlid;

static game::globals_t _globals {};
game::globals_t const & game::globals {_globals};

static game::realm * root_realm = nullptr;

game::control_interface game::ci;

void game::init() {
	root_realm = new realm {};
}

void game::term() {
	delete root_realm;
}

void game::frame(as::time::span const & ft) {
	_globals.impulse = ft.sec<double>();
	_globals.msec += ft.msec<uint_fast64_t>();
	root_realm->frame();
	ci = {};
}

game::realm * game::get_root_realm() {
	return root_realm;
}
