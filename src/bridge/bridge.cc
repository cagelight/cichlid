#include "bridge.hh"

#include "text.hh"

void ci::bridge::init() {
	text::init();
}

void ci::bridge::term() noexcept {
	text::term();
}
