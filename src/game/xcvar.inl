// CONSOLE VARIABLES -- GAME

#ifdef CVAR_EXTERNDECL
#define CICVAR_SINT(cmd_name, default_value) extern int_fast64_t cmd_name;
#define CICVAR_UINT(cmd_name, default_value) extern uint_fast64_t cmd_name;
#define CICVAR_REAL(cmd_name, default_value) extern ci::real_t cmd_name;
#define CICVAR_VEC3(cmd_name, default_value) extern ci::vec3_t cmd_name;
#endif

#ifdef CVAR_DECL
#define CICVAR_SINT(cmd_name, default_value) int_fast64_t ci::game::cmd_name = default_value;
#define CICVAR_UINT(cmd_name, default_value) uint_fast64_t ci::game::cmd_name = default_value;
#define CICVAR_REAL(cmd_name, default_value) ci::real_t ci::game::cmd_name = default_value;
#define CICVAR_VEC3(cmd_name, default_value) ci::vec3_t ci::game::cmd_name = {default_value};
#endif

#ifdef CVAR_REGISTER
#define CICVAR_SINT(cmd_name, default_value) ci::com::cmd.register_command(#cmd_name, [](std::string const & vstr){ if (!vstr.size()) scilogi << #cmd_name ": " << cmd_name << " (default: " #default_value ")"; else cmd_name = strtoll(vstr.c_str(), nullptr, 10); });
#define CICVAR_UINT(cmd_name, default_value) ci::com::cmd.register_command(#cmd_name, [](std::string const & vstr){ if (!vstr.size()) scilogi << #cmd_name ": " << cmd_name << " (default: " #default_value ")"; else cmd_name = strtoull(vstr.c_str(), nullptr, 10); });
#define CICVAR_REAL(cmd_name, default_value) ci::com::cmd.register_command(#cmd_name, [](std::string const & vstr){ if (!vstr.size()) scilogi << #cmd_name ": " << cmd_name << " (default: " #default_value ")"; else cmd_name = strtod(vstr.c_str(), nullptr); });
#define CICVAR_VEC3(cmd_name, default_value) ci::com::cmd.register_command(#cmd_name, [](std::string const & vstr){ if (!vstr.size()) scilogi << #cmd_name ": " << cmd_name << " (default: " << vec3_t {default_value} << ")";\
	else {\
		auto split = as::strop::separate(vstr, ' ', 3);\
		switch (split.size()){\
			default: sciloge << "invalid number of arguments"; break;\
			case 1: cmd_name = strtod(vstr.c_str(), nullptr); break;\
			case 3: cmd_name = {strtod(split[0].c_str(), nullptr), strtod(split[1].c_str(), nullptr), strtod(split[2].c_str(), nullptr)}; break;\
		}\
	}});
#endif

CICVAR_VEC3(g_gravity_vector, 0 COMMA -1 COMMA 0)
CICVAR_UINT(g_log_player_transform, 0)

#undef CICVAR_SINT
#undef CICVAR_UINT
#undef CICVAR_REAL
#undef CICVAR_VEC3

#undef CVAR_EXTERNDECL
#undef CVAR_DECL
#undef CVAR_REGISTER
