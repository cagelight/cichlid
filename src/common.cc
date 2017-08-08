#include "common.hh"

#include <iostream>
#include <mutex>

using namespace cichlid;

std::atomic_bool com::run_sem {false};

void com::init() {
	run_sem.store(true);
}

void com::term() {
	
}

static std::mutex log_mut {};
void com::log( std::string const & str ) {
	log_mut.lock();
	std::cout << str << std::endl;
	log_mut.unlock();
}
