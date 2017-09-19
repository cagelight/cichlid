#pragma once

#include <asterid/strops.hh>
#include <asterid/synchro.hh>
#include <asterid/brassica.hh>
#include <asterid/time.hh>
#include <asterid/byte_buffer.hh>

#include <cstdint>

#include <atomic>
#include <string>
#include <unordered_map>
#include <map>
#include <string_view>

// disgustingly hacky but necessary to pass commas into macros
#define COMMA ,

namespace as = asterid;
namespace b = asterid::brassica;

namespace cichlid {
	
	typedef double real_t;
	
	namespace com {
		
		namespace exception {
			struct startup {};
		}
	
		extern std::atomic_bool run_sem;
		
		void init();
		void term();
		
		enum struct log_level {
			info,
			warning,
			error,
			fatal,
			debug
		};
		void log(log_level, std::string const &);
	
		struct console {
			// no const reference access to underling buffers, thread race issues
			std::vector<std::string> history(size_t max = SIZE_MAX) const;
			std::vector<std::string> log(size_t max = SIZE_MAX) const;
			// ====
			console() = default;
			
			bool exec(std::string const &);
			void register_command(std::string const &, std::function<void(std::string const &)>);
			void add_log_entry(std::string const &);
			void clear();
			std::vector<std::string> autocomplete_options(std::string const & str);
		private:
			std::vector<std::string> history_ {};
			std::vector<std::string> log_ {};
			std::map<std::string, std::function<void(std::string const &)>> cmds;
			mutable asterid::spinlock buffer_access_lk;
		};
		
		extern console cmd;
	}
	
	typedef b::vec2_t<real_t> vec2_t;
	typedef b::vec3_t<real_t> vec3_t;
	typedef b::rect_t<real_t> rect_t;
	typedef b::quaternion_t<real_t> quat_t;
	
	typedef b::mat4_t<real_t> mat4_t;
	
	static constexpr real_t pi = b::pi<real_t>;
	static constexpr real_t pi_h = b::pi<real_t> / static_cast<real_t>(2);
	
	struct transformation_t {
		vec3_t pos {};
		quat_t rot {};
		vec3_t scale {1};
		inline mat4_t to_matrix() const {
			return mat4_t::scale(scale) * mat4_t(rot) * mat4_t::translate(pos);
		}
	};
	
	// ====
	
	namespace logging_internal {
		
		struct cilogger {
			com::log_level level;
			std::string str;
			inline cilogger(com::log_level lev, std::string const & init = "") : level(lev), str(init) {}
			inline ~cilogger() { com::log(level, str); }
			
			inline cilogger & operator << ( std::string const & other ) { str += other; return *this; }
			inline cilogger & operator << ( char const * other ) { str += other; return *this; }
			template <typename T> inline cilogger & operator << ( T const & other ) { str += std::to_string(other); return *this; }
		};
		
		struct cilogger_dummy {
			inline cilogger_dummy() {}
			template <typename T> inline cilogger_dummy & operator << ( T const & ) {return *this; }
		};
	}
}

#define BRIDGE_PUBLIC "bridge/^external.hh"
#define BRIDGE_PRIVATE "bridge/^internal.hh"
#define CONTROL_PUBLIC "control/^external.hh"
#define CONTROL_PRIVATE "control/^internal.hh"
#define GAME_PUBLIC "game/^external.hh"
#define GAME_PRIVATE "game/^internal.hh"
#define RENDER_PUBLIC "render/^external.hh"
#define RENDER_PRIVATE "render/^internal.hh"

#define INTERNAL_LEAK_FLAW static_assert(false, "internal file used in external context");

#define cilog(prefix, lev, fmt, ...) cichlid::com::log(lev, as::strf("%s: %s", #prefix, as::vas(fmt, ##__VA_ARGS__)))
#define cilogv(prefix, lev, fmt, ...) cichlid::com::log(lev, as::strf("%s (%s): %s", #prefix, _as_here, as::vas(fmt, ##__VA_ARGS__)))
#define cilogi(...) cilog(INFO, cichlid::com::log_level::info, ##__VA_ARGS__)
#define cilogvi(...) cilogv(INFO, cichlid::com::log_level::info, ##__VA_ARGS__)
#define cilogw(...) cilog(WARNING, cichlid::com::log_level::warning, ##__VA_ARGS__)
#define cilogvw(...) cilogv(WARNING, cichlid::com::log_level::warning, ##__VA_ARGS__)
#define ciloge(...) cilog(ERROR, cichlid::com::log_level::error, ##__VA_ARGS__)
#define cilogve(...) cilogv(ERROR, cichlid::com::log_level::error, ##__VA_ARGS__)
#ifdef CICHLID_DEBUG
	#define cilogb(...) cilog(DEBUG, cichlid::com::log_level::debug, ##__VA_ARGS__)
	#define cilogvb(...) cilogv(DEBUG, cichlid::com::log_level::debug, ##__VA_ARGS__)
#else
	#define cilogb(...)
	#define cilogvb(...)
#endif

#define scilog(prefix, lev) cichlid::logging_internal::cilogger { lev, #prefix ": " }
#define scilogv(prefix, lev) cichlid::logging_internal::cilogger { lev, as::strf("%s (%s): ", #prefix, _as_here) }
#define scilogi scilog(INFO, cichlid::com::log_level::info)
#define scilogvi scilogv(INFO, cichlid::com::log_level::info)
#define scilogw scilog(WARNING, cichlid::com::log_level::warning)
#define scilogvw scilogv(WARNING, cichlid::com::log_level::warning)
#define sciloge scilog(ERROR, cichlid::com::log_level::error)
#define scilogve scilogv(ERROR, cichlid::com::log_level::error)
#ifdef CICHLID_DEBUG
	#define scilogb scilog(DEBUG, cichlid::com::log_level::debug)
	#define scilogvb scilogv(DEBUG, cichlid::com::log_level::debug)
#else
	#define scilogb cichlid::logging_internal::cilogger_dummy {}
	#define scilogvb cichlid::logging_internal::cilogger_dummy {}
#endif

#define except(type) cichlid::com::exception::type
#define throwe(type) throw except(type) {}
