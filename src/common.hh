#pragma once

#include <asterid/strops.hh>
#include <asterid/synchro.hh>
#include <asterid/brassica.hh>
#include <asterid/time.hh>

#include <cstdint>

#include <atomic>
#include <string>

namespace as = asterid;
namespace b = asterid::brassica;

namespace cichlid::com {
	
	extern std::atomic_bool run_sem;
	
	void init();
	void term();
	
	void log(std::string const &);
	
	namespace exception {
		struct startup {};
	}
	
}

#define cilog(prefix, fmt, ...) cichlid::com::log(as::strf("%s: %s", #prefix, as::vas(fmt, ##__VA_ARGS__)))
#define cilogv(prefix, fmt, ...) cichlid::com::log(as::strf("%s (%s): %s", #prefix, here, as::vas(fmt, ##__VA_ARGS__)))
#define cilogi(...) cilog(INFO, ##__VA_ARGS__)
#define cilogvi(...) cilogv(INFO, ##__VA_ARGS__)
#define cilogw(...) cilog(WARNING, ##__VA_ARGS__)
#define cilogvw(...) cilogv(WARNING, ##__VA_ARGS__)
#define ciloge(...) cilog(ERROR, ##__VA_ARGS__)
#define cilogve(...) cilogv(ERROR, ##__VA_ARGS__)
#ifdef CICHLID_DEBUG
	#define cilogb(...) cilog(DEBUG, ##__VA_ARGS__)
	#define cilogvb(...) cilogv(DEBUG, ##__VA_ARGS__)
#else
	#define cilogb(...)
	#define cilogvb(...)
#endif

#define except(type) cichlid::com::exception::type
#define throwe(type) throw except(type) {}
