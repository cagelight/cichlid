#include "core.hh"

#include <csignal>

using namespace cichlid;

void handle_signal(int sig) {
	switch (sig) {
		default:
			return;
		case SIGINT:
			cilogvw("SIGINT received");
			if (com::run_sem) com::run_sem.store(false);
			else std::terminate();
	}
}

int main() {
	signal(SIGINT, handle_signal);
	core::exec();
	return 0;
}
