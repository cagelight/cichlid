#include "core.hh"

#include "control/control.hh"
#include "render/render.hh"
#include "game/game.hh"

#include <thread>

using namespace cichlid;

void core::init() {
	com::init();
	control::init();
	render::init();
}

void core::term() {
	render::term();
	control::term();
	com::term();
}

void core::exec() {
	
	as::time::keeper<as::time::clock_type::monotonic> coretk {};
	as::time::span frametime {1.0 / 240.0};
	
	try {
		com::init();
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
	
	control::term();
	game::term();
	com::term();
}
