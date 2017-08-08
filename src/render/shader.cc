#include "shader.hh"

#include "bridge/glfw_common.hh"

namespace cichlid::render::shader {
	struct stage final {
		GLuint id;
		stage(GLenum type) : id(glCreateShader(type)) {}
		~stage() { glDeleteShader(id); }
		void source(char const * src) { glShaderSource(id, 1, &src, 0); }
		bool compile(std::string & complog) {
			complog.clear();
			GLint success, maxlen;
			glCompileShader(id);
			glGetShaderiv(id, GL_COMPILE_STATUS, &success);
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxlen);
			if (!maxlen) return success == GL_TRUE;
			char * logtemp = new char [maxlen + 1];
			glGetShaderInfoLog(id, maxlen, &maxlen, logtemp);
			complog = logtemp;
			delete [] logtemp;
			return success == GL_TRUE;
		}
	};
	
	struct program final {
		GLuint id;
		program() : id(glCreateProgram()) {}
		void attach(stage const & st) { glAttachShader(id, st.id); }
		bool link(std::string & complog) {
			complog.clear();
			GLint success, maxlen;
			glLinkProgram(id);
			glGetProgramiv(id, GL_LINK_STATUS, &success);
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxlen);
			if (!maxlen) return success == GL_TRUE;
			char * logtemp = new char [maxlen + 1];
			glGetProgramInfoLog(id, maxlen, &maxlen, logtemp);
			complog = logtemp;
			delete [] logtemp;
			return success == GL_TRUE;
		}
	};
}

using namespace cichlid::render;


static char const basic_v_src [] = 
	"#version 450 core \n"
	"layout(location = 0) in vec3 pos;"
	"layout(location = 1) in vec2 uv_in;"
	"layout(location = 2) uniform mat4 mvp;"
	"out vec2 uv;"
	"void main() {"
		"uv = uv_in;"
		"gl_Position = mvp * vec4(pos, 1);"
	"}";
	
static char const basic_f_src [] = 
	"#version 450 core \n"
	"in vec2 uv;"
	"out vec3 frag_color;"
	"void main() {"
		"frag_color = vec3(0, 1, 0);"
	"}";
	
static std::unique_ptr<shader::program> basic_prog;

static void init_basic() {
	std::string complog;
	
	shader::stage basic_v {GL_VERTEX_SHADER};
	basic_v.source(basic_v_src);
	if (!basic_v.compile(complog)) {
		cilogve("basic vertex shader failed to compile:\n%s", complog.c_str());
	}
	
	shader::stage basic_f {GL_FRAGMENT_SHADER};
	basic_f.source(basic_f_src);
	if (!basic_f.compile(complog)) {
		cilogve("basic fragment shader failed to compile:\n%s", complog.c_str());
	}
	
	basic_prog.reset(new shader::program {});
	basic_prog->attach(basic_v);
	basic_prog->attach(basic_f);
	if (!basic_prog->link(complog)) {
		cilogve("basic shader program failed to link:\n%s", complog.c_str());
	}
}

static void term_basic() {
	basic_prog.reset();
}

void shader::init() {
	init_basic();
}

void shader::term() {
	term_basic();
}

void shader::bind(shader::type t) {
	switch (t) {
		case shader::type::basic:
			glUseProgram(basic_prog->id);
			break;
		default:
			break;
	}
}

void shader::upload(shader::basic const & b) {
	glUniformMatrix4fv(2, 1, GL_FALSE, b.m);
}
