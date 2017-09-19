// CONSOLE VARIABLES -- RENDER

#ifdef CVAR_EXTERNDECL
#define CICVAR_SINT(cmd_name, default) extern int_fast64_t cmd_name;
#define CICVAR_UINT(cmd_name, default) extern uint_fast64_t cmd_name;
#define CICVAR_REAL(cmd_name, default) extern real_t cmd_name;
#endif

#ifdef CVAR_DECL
#define CICVAR_SINT(cmd_name, default) int_fast64_t render::cmd_name = default;
#define CICVAR_UINT(cmd_name, default) uint_fast64_t render::cmd_name = default;
#define CICVAR_REAL(cmd_name, default) real_t render::cmd_name = default;
#endif

#ifdef CVAR_REGISTER
#define CICVAR_SINT(cmd_name, default) com::cmd.register_command(#cmd_name, [](std::string const & vstr){ if (!vstr.size()) scilogi << #cmd_name ": " << cmd_name << " (default: " #default ")"; else cmd_name = strtoll(vstr.c_str(), nullptr, 10); });
#define CICVAR_UINT(cmd_name, default) com::cmd.register_command(#cmd_name, [](std::string const & vstr){ if (!vstr.size()) scilogi << #cmd_name ": " << cmd_name << " (default: " #default ")"; else cmd_name = strtoull(vstr.c_str(), nullptr, 10); });
#define CICVAR_REAL(cmd_name, default) com::cmd.register_command(#cmd_name, [](std::string const & vstr){ if (!vstr.size()) scilogi << #cmd_name ": " << cmd_name << " (default: " #default ")"; else cmd_name = strtod(vstr.c_str(), nullptr); });
#endif

CICVAR_UINT(r_origin_lines, 1)
CICVAR_REAL(r_vfov, 90.0)

#undef CICVAR_SINT
#undef CICVAR_UINT
#undef CICVAR_REAL

#undef CVAR_EXTERNDECL
#undef CVAR_DECL
#undef CVAR_REGISTER
