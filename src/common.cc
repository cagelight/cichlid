#include "common.hh"

#include <iostream>
#include <mutex>

using namespace cichlid;

std::atomic_bool com::run_sem {false};
com::console com::cmd {};

void com::init() {
	run_sem.store(true);
	cmd.register_command("quit", [](std::string const &){ run_sem.store(false); });
	cmd.register_command("clear", [](std::string const &){ cmd.clear(); });
}

void com::term() {
	
}

static std::mutex log_mut {};
void com::log(log_level, std::string const & str) {
	log_mut.lock();
	std::cout << str << std::endl;
	cmd.add_log_entry(str);
	log_mut.unlock();
}

std::vector<std::string> com::console::history(size_t max) const {
	buffer_access_lk.lock();
	if (max > history_.size()) max = history_.size();
	std::vector<std::string> vec {history_.rbegin(), history_.rbegin() + max};
	buffer_access_lk.unlock();
	return vec;
}

std::vector<std::string> com::console::log(size_t max) const {
	buffer_access_lk.lock();
	if (max > log_.size()) max = log_.size();
	std::vector<std::string> vec {log_.rbegin(), log_.rbegin() + max};
	buffer_access_lk.unlock();
	return vec;
}

bool com::console::exec(std::string const & cmd) {
	history_.push_back(cmd);
	scilog(CMD, cichlid::com::log_level::info) << cmd;
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

void com::console::register_command(std::string const & cmd, std::function<void(std::string const &)> func) {
	cmds[cmd] = func;
}

void com::console::add_log_entry(std::string const & str) {
	buffer_access_lk.lock();
	log_.push_back(str);
	if (log_.size() > 500) log_.erase(log_.begin(), log_.begin() + (log_.size() - 500));
	buffer_access_lk.unlock();
}

void com::console::clear() {
	buffer_access_lk.lock();
	log_.clear();
	buffer_access_lk.unlock();
}

std::vector<std::string> com::console::autocomplete_options(std::string const & str) {
	std::vector<std::string> options {};
	for (auto const & i : cmds) {
		if (as::strop::starts_width(i.first, str)) {
			options.push_back(i.first);
		}
	}
	return options;
}

