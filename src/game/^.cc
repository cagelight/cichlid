#define GAME_SOURCE
#include "common.hh"
#include GAME_PRIVATE

#include "realm.hh"

using namespace cichlid;

game::globals_t game::globals_ {};
game::globals_t const & game::globals {globals_};

std::mt19937_64 game::rng {std::random_device{}()};

static game::realm * root_realm = nullptr;

game::control_interface game::ci;

void game::init() {
	root_realm = new realm {};
	
	#define CVAR_REGISTER
	#include "^cvar.inl"
}

void game::term() {
	delete root_realm;
}

void game::frame(as::time::span const & ft) {
	globals_.impulse = ft.sec<double>();
	globals_.msec += ft.msec<uint_fast64_t>();
	
	if (ci.trigger.console_toggle) {
		globals_.in_console = !globals_.in_console;
	} else if (globals_.in_console) {
		
		if (ci.trigger.console_history_up) {
			if (globals.history_index == 0) {
				globals_.console_cache_text = globals_.console_text;
			}
			globals_.history_index++;
			auto history = com::cmd.history(globals.history_index);
			if (history.size()) {
				if (globals.history_index > history.size()) globals_.history_index = history.size();
				globals_.console_text = history[globals.history_index - 1];
			} else globals_.history_index = 0;
		} else if (ci.trigger.console_history_down) {
			if (globals.history_index != 0) {
				globals_.history_index--;
				if (globals.history_index == 0) {
					globals_.console_text = globals_.console_cache_text;
				} else {
					auto history = com::cmd.history(globals.history_index);
					if (history.size()) {
						if (globals.history_index > history.size()) globals_.history_index = history.size();
						globals_.console_text = history[globals.history_index - 1];
					} else {
						globals_.history_index = 0;
						globals_.console_text = globals_.console_cache_text;
					}
				}
			}
		}
		
		if (ci.trigger.text_backspace && globals_.console_text.size()) {
			globals_.console_text.erase(globals_.console_text.end() - 1);
			globals_.show_autocomplete = false;
			globals_.history_index = 0;
		}
		if (ci.ascii_input.size()) {
			globals_.console_text += ci.ascii_input;
			globals_.show_autocomplete = false;
			globals_.history_index = 0;
		}
		if (ci.trigger.console_autocomplete) {
			if (globals.show_autocomplete) {
				globals_.console_text = globals.console_autocomplete_options[0] + ' ';
				globals_.show_autocomplete = false;
				globals_.history_index = 0;
			} else {
				globals_.history_index = 0;
				globals_.console_autocomplete_options = com::cmd.autocomplete_options(globals.console_text);
				if (globals.console_autocomplete_options.size() > 1) {
					std::string autocomplete_min = globals.console_autocomplete_options[0];
					for (std::string const & test : globals.console_autocomplete_options) {
						typename std::string::const_iterator a_i = autocomplete_min.begin(), b_i = test.begin();
						for (;a_i != autocomplete_min.end() && b_i != test.end(); a_i++, b_i++) if (*a_i != *b_i) break;
						if (a_i != autocomplete_min.end()) autocomplete_min = {autocomplete_min.cbegin(), a_i};
					}
					globals_.console_text = autocomplete_min;
					globals_.show_autocomplete = true;
				}
				else if (globals.console_autocomplete_options.size() == 1) globals_.console_text = globals.console_autocomplete_options[0] + ' ';
			}
		}
		if (ci.trigger.console_enter) {
			if (globals_.console_text.size()) com::cmd.exec(globals_.console_text);
			globals_.console_text.clear();
			globals_.show_autocomplete = false;
			globals_.history_index = 0;
		}
		ci = {};
	}
	
	root_realm->frame();
	ci = {};
}

game::realm * game::get_root_realm() {
	return root_realm;
}

#define CVAR_DECL
#include "^cvar.inl"
