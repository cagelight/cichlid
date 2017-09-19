#include "core.hh"

#include "common.hh"
#include BRIDGE_PUBLIC
#include CONTROL_PUBLIC
#include GAME_PUBLIC
#include RENDER_PUBLIC

#include <thread>

using namespace cichlid;

void core::exec() {
	
	as::time::keeper<as::time::clock_type::monotonic> coretk {};
	as::time::span frametime {1.0 / 240.0};
	
	try {
		com::init();
		bridge::init();
		game::init();
		control::init();
	
		as::fifo_mutex gmut;
		
		std::thread * render_thr = new std::thread([&](){
			try {
				render::init();
				while (com::run_sem) {
					gmut.lock();
					render::preframe();
					gmut.unlock();
					render::frame();
				}
			} catch (except(startup)) {
				cilogve("fatal exception occured during render initialization");
			}
			com::run_sem.store(false);
			render::term();
		});
		
		coretk.mark();
		
		while (com::run_sem) {
			as::time::span ft = coretk.sleep_mark(frametime);
			
			gmut.lock();
			control::frame();
			game::frame(ft);
			gmut.unlock();
		}
		
		render_thr->join();
		delete render_thr;
	
	} catch (except(startup)) {
		cilogve("fatal exception occured during startup");
	}

	bridge::term();
	control::term();
	game::term();
	com::term();
}
