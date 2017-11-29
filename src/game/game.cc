#include "realm.hh"
#include "game_private.hh"

ci::game::globals_t ci::game::globals_ {};
ci::game::globals_t const & ci::game::globals {globals_};

ci::game::control_interface ci::game::conint;

std::mt19937_64 ci::game::rng {std::random_device{}()};

#define CVAR_DECL
#include "xcvar.inl"

std::unique_ptr<ci::game::realm> root_realm;

void ci::game::init() {
	#define CVAR_REGISTER
	#include "xcvar.inl"
	
	root_realm = std::unique_ptr<ci::game::realm> {new realm};
}

void ci::game::term() noexcept {
	root_realm.reset();
}

static void console_frame() {
	using namespace ci;
	using namespace ci::game;
	
	if (conint.trigger.console_toggle) {
		globals_.in_console = !globals_.in_console;
	} else if (globals_.in_console) {
		
		if (conint.trigger.console_history_up) {
			if (globals.history_index == 0) {
				globals_.console_cache_text = globals_.console_text;
			}
			globals_.history_index++;
			auto history = com::cmd.history(globals.history_index);
			if (history.size()) {
				if (globals.history_index > history.size()) globals_.history_index = history.size();
				globals_.console_text = history[globals.history_index - 1];
			} else globals_.history_index = 0;
		} else if (conint.trigger.console_history_down) {
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
		
		if (conint.trigger.text_backspace && globals_.console_text.size()) {
			globals_.console_text.erase(globals_.console_text.end() - 1);
			globals_.show_autocomplete = false;
			globals_.history_index = 0;
		}
		if (conint.ascii_input.size()) {
			globals_.console_text += conint.ascii_input;
			globals_.show_autocomplete = false;
			globals_.history_index = 0;
		}
		if (conint.trigger.console_autocomplete) {
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
		if (conint.trigger.console_enter) {
			if (globals_.console_text.size()) com::cmd.exec(globals_.console_text);
			globals_.console_text.clear();
			globals_.show_autocomplete = false;
			globals_.history_index = 0;
		}
		conint = {};
	}
};

void ci::game::frame(as::time::span const & ft) {
	globals_.impulse = ft.sec<double>();
	globals_.msec += ft.msec<uint_fast64_t>();
	
	console_frame();
	root_realm->frame();
	
	conint = {};
}

ci::game::realm const & ci::game::get_realm() {
	if (!root_realm) throwe(uninitialized);
	return *root_realm;
}
