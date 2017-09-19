#define BRIDGE_SOURCE
#include "common.hh"
#include BRIDGE_PRIVATE

#include "text.hh"

using namespace cichlid;

void bridge::init() {
	text::init();
}

void bridge::term() noexcept {
	text::term();
}
