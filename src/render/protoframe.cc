#define RENDER_SOURCE
#include "common.hh"
#include RENDER_PRIVATE

#include "protoframe.hh"

using namespace cichlid;

render::protoframe::protoframe(game::realm const & r, game::perspective const & p) {
	pers = p;
	for (auto const & c : r.cubes) {
		this->cubes.emplace_back(c);
	}
}
