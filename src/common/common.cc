#include <iostream>
#include <csignal>

#include "common.hh"

std::atomic_bool ci::com::run_sem {false};
ci::com::console ci::com::cmd {};

void ci::com::init() {
	run_sem.store(true);
	cmd.register_command("quit", [](std::string const &){ run_sem.store(false); });
	cmd.register_command("clear", [](std::string const &){ cmd.clear(); });
}

void ci::com::term() {
	
}

void handle_signal(int sig) {
	switch (sig) {
		default:
			return;
		case SIGINT:
			scilogvw << "SIGINT received";
			if (ci::com::run_sem) ci::com::run_sem.store(false);
			else std::terminate();
	}
}

static std::mutex log_mut {};
void ci::com::log(log_level, std::string const & str) {
	log_mut.lock();
	std::cout << str << std::endl;
	cmd.add_log_entry(str);
	log_mut.unlock();
}

std::vector<std::string> ci::com::console::history(size_t max) const {
	buffer_access_lk.lock();
	if (max > history_.size()) max = history_.size();
	std::vector<std::string> vec {history_.rbegin(), history_.rbegin() + max};
	buffer_access_lk.unlock();
	return vec;
}

std::vector<std::string> ci::com::console::log(size_t max) const {
	buffer_access_lk.lock();
	if (max > log_.size()) max = log_.size();
	std::vector<std::string> vec {log_.rbegin(), log_.rbegin() + max};
	buffer_access_lk.unlock();
	return vec;
}

bool ci::com::console::exec(std::string const & cmd) {
	history_.push_back(cmd);
	scilog(CMD, ci::com::log_level::info) << cmd;
	auto cmd_split = as::strop::separate(cmd, ' ', 1);
	auto const & fcmd = cmds.find(cmd_split[0]);
	if (fcmd != cmds.end()) {
		fcmd->second(cmd_split.size() == 2 ? cmd_split[1] : "");
	}
	else {
		scilogi << "unknown command";
	}
	
	return false;
}

void ci::com::console::register_command(std::string const & cmd, std::function<void(std::string const &)> func) {
	cmds[cmd] = func;
}

void ci::com::console::add_log_entry(std::string const & str) {
	buffer_access_lk.lock();
	log_.push_back(str);
	if (log_.size() > 500) log_.erase(log_.begin(), log_.begin() + (log_.size() - 500));
	buffer_access_lk.unlock();
}

void ci::com::console::clear() {
	buffer_access_lk.lock();
	log_.clear();
	buffer_access_lk.unlock();
}

std::vector<std::string> ci::com::console::autocomplete_options(std::string const & str) {
	std::vector<std::string> options {};
	for (auto const & i : cmds) {
		if (as::strop::starts_width(i.first, str)) {
			options.push_back(i.first);
		}
	}
	return options;
}

#include "bridge/bridge.hh"
#include "game/game.hh"
#include "control/control.hh"
#include "render/render.hh"

int main() {
	signal(SIGINT, handle_signal);
	
	try {
		ci::com::init();
		ci::bridge::init();
		ci::game::init();
		ci::control::init();
		
		as::time::keeper<as::time::clock_type::monotonic> coretk {};
		as::time::span frametime {1.0 / 240.0};
		as::fifo_mutex gmut;
		
		std::thread * render_thr = new std::thread([&](){
			try {
				ci::render::init();
				while (ci::com::run_sem) {
					gmut.lock();
					ci::render::preframe();
					gmut.unlock();
					ci::render::frame();
				}
			} catche(startup) {
				scilogve << "fatal exception occured during render initialization";
			} catch(...) {
				
			}
			ci::com::run_sem.store(false);
			ci::render::term();
		});
		
		coretk.mark();
		while (ci::com::run_sem) {
			as::time::span ft = coretk.sleep_mark(frametime);
			
			gmut.lock();
			ci::control::frame();
			ci::game::frame(ft);
			gmut.unlock();
		}
		
		
		render_thr->join();
		delete render_thr;
		
	} catche(startup) {
		scilogve << "fatal exception occured during initialization";
	} catch(...) {
		
	}
	
	ci::control::term();
	ci::game::term();
	ci::bridge::term();
	ci::com::term();
	
	return 0;
}
