#pragma once

#include <asterid/strops.hh>
#include <asterid/synchro.hh>
#include <asterid/brassica.hh>
#include <asterid/time.hh>
#include <asterid/buffer_assembly.hh>

#include <cstdint>

#include <atomic>
#include <string>
#include <unordered_map>
#include <map>
#include <string_view>
#include <iomanip>

// trash
#undef major
#undef minor

// disgustingly hacky but necessary to pass commas into macros
#define COMMA ,

namespace as = asterid;
namespace b = asterid::brassica;

typedef double real_t;

namespace ci {

	namespace com {
		
		namespace exception {
			struct startup {};
			struct uninitialized {};
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
			
		struct cilogger {
			com::log_level level;
			std::string str;
			inline cilogger(com::log_level lev, std::string const & init = "") : level(lev), str(init) {}
			inline ~cilogger() { ci::com::log(level, str); }
			
			inline cilogger & operator << ( std::string const & other ) { str += other; return *this; }
			inline cilogger & operator << ( char const * other ) { str += other; return *this; }
			template <typename T> inline cilogger & operator << ( T const & other ) { str += std::to_string(other); return *this; }
		};

		struct cilogger_dummy {
			inline cilogger_dummy() {}
			template <typename T> inline cilogger_dummy & operator << ( T const & ) {return *this; }
		};
	}

	typedef b::vec2_t<real_t> vec2_t;
	typedef b::vec3_t<real_t> vec3_t;
	typedef b::rect_t<real_t> rect_t;
	typedef b::quaternion_t<real_t> quat_t;

	typedef b::mat4_t<real_t> mat4_t;

	static constexpr real_t pi = b::pi<real_t>;
	static constexpr real_t pi_h = b::pi<real_t> / static_cast<real_t>(2);
	static constexpr real_t pi_2 = b::pi<real_t> * static_cast<real_t>(2);
	
	struct length_t {
		uint_fast64_t major = 0;
		real_t minor = 0;
		
		inline length_t() = default;
		inline length_t(length_t const &) = default;
		inline length_t(length_t &&) = default;
		inline length_t(uint_fast64_t a, real_t b) : major(a), minor(b) { settle(); }
		inline length_t(real_t v) : length_t(0, v) {}
		
		inline void settle() {
			uint_fast64_t inc = std::floor(minor);
			major += inc;
			minor -= inc;
		}
		
		inline real_t squash() { return static_cast<real_t>(*this); }
		
		inline explicit operator real_t () { return minor + major; }
		
		inline length_t operator - () { return {-major, -minor}; }
		inline length_t & operator = (length_t const & other) { major = other.major; minor = other.minor; return *this; }
		inline bool operator == (length_t const & other) { return major == other.major && minor == other.minor; }
		inline length_t operator + (length_t const & other) { return {major + other.major, minor + other.minor}; }
		inline length_t operator - (length_t const & other) { return {major - other.major, minor - other.minor}; }
		inline length_t & operator += (length_t const & other) { major += other.major; minor += other.minor; settle(); return *this; }
		inline length_t & operator -= (length_t const & other) { major -= other.major; minor -= other.minor; settle(); return *this; }
	};
	
	typedef b::vec3_t<length_t> pos_t;

	struct transformation_t {
		vec3_t pos {};
		quat_t rot {};
		vec3_t scale {1};
		inline mat4_t to_matrix() const {
			return mat4_t::scale(scale) * mat4_t(rot) * mat4_t::translate(pos);
		}
		
		static inline transformation_t lerp(transformation_t const & A, transformation_t const & B, double v) {
			transformation_t r;
			r.pos = b::lerp<vec3_t>(A.pos, B.pos, v);
			r.scale = b::lerp<vec3_t>(A.scale, B.scale, v);
			r.rot = quat_t::slerp(A.rot, B.rot, v);
			return r;
		}
	};

}

// ====
	
#define scilog(prefix, lev) ci::com::cilogger { lev, #prefix ": " }
#define scilogv(prefix, lev) ci::com::cilogger { lev, as::strf("%s (%s): ", #prefix, _as_here) }
#define scilogi scilog(INFO, ci::com::log_level::info)
#define scilogvi scilogv(INFO, ci::com::log_level::info)
#define scilogw scilog(WARNING, ci::com::log_level::warning)
#define scilogvw scilogv(WARNING, ci::com::log_level::warning)
#define sciloge scilog(ERROR, ci::com::log_level::error)
#define scilogve scilogv(ERROR, ci::com::log_level::error)
#ifdef CICHLID_DEBUG
	#define scilogb scilog(DEBUG, ci::com::log_level::debug)
	#define scilogvb scilogv(DEBUG, ci::com::log_level::debug)
#else
	#define scilogb ci::com::cilogger_dummy {}
	#define scilogvb ci::com::cilogger_dummy {}
#endif

#define catche(type) catch (ci::com::exception::type &)
#define throwe(type) throw ci::com::exception::type {}
