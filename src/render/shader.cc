#define RENDER_SOURCE
#include "common.hh"
#include RENDER_PRIVATE

#include "shader.hh"

#define SS(v) _SS(v)
#define _SS(v) #v

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


// BASIC

static char const basic_v_src [] = R"SHAD(
	#version 450 core
	layout(location = )SHAD" SS(SHADER_LOCATION_VERTEX) R"SHAD() in vec3 pos;
	layout(location = )SHAD" SS(SHADER_LOCATION_UV) R"SHAD() in vec2 uv_in;
	layout(location = )SHAD" SS(SHADER_LOCATION_MVP) R"SHAD() uniform mat4 mvp;
	out vec2 uv;
	void main() {
		uv = uv_in;
		gl_Position = mvp * vec4(pos, 1);
	};
)SHAD";
	
static char const basic_f_src [] = R"SHAD(
	#version 450 core
	in vec2 uv;
	layout(location = )SHAD" SS(SHADER_LOCATION_COLOR) R"SHAD() uniform vec4 color;
	out vec4 frag_color;
	void main() {
		frag_color = color;
	};
)SHAD";

// LINE

static char const line_v_src [] = R"SHAD(
	#version 450 core
	layout(location = )SHAD" SS(SHADER_LOCATION_VERTEX) R"SHAD() in vec3 pos;
	layout(location = )SHAD" SS(SHADER_LOCATION_MVP) R"SHAD() uniform mat4 mvp;
	void main() {
		gl_Position = mvp * vec4(pos, 1);
	};
)SHAD";
	
static char const line_f_src [] = R"SHAD(
	#version 450 core
	layout(location = )SHAD" SS(SHADER_LOCATION_COLOR) R"SHAD() uniform vec4 color;
	out vec4 frag_color;
	void main() {
		frag_color = color;
	};
)SHAD";

// TEXT
	
static char const text_v_src [] = R"SHAD(
	#version 450 core
	layout(location = )SHAD" SS(SHADER_LOCATION_VERTEX) R"SHAD() in vec3 pos;
	layout(location = )SHAD" SS(SHADER_LOCATION_UV) R"SHAD() in vec2 uv_in;
	layout(location = )SHAD" SS(SHADER_LOCATION_MVP) R"SHAD() uniform mat4 mvp;
	layout(location = )SHAD" SS(SHADER_LOCATION_OFFS2D) R"SHAD() uniform vec2 offs;
	layout(location = )SHAD" SS(SHADER_LOCATION_SCALE2D) R"SHAD() uniform vec2 scale;
	out vec2 uv;
	void main() {
		uv = uv_in;
		gl_Position = mvp * vec4(scale.x * pos.x + offs.x, scale.y * pos.y + offs.y, pos.z, 1);
	};
)SHAD";
	
static char const text_f_src [] = R"SHAD(
	#version 450 core
	in vec2 uv;
	layout(location = )SHAD" SS(SHADER_LOCATION_COLOR) R"SHAD() uniform vec4 color;
	layout(location = )SHAD" SS(SHADER_LOCATION_TEXOFFS) R"SHAD() uniform vec2 uv_offs;
	layout(location = )SHAD" SS(SHADER_LOCATION_TEXSCALE) R"SHAD() uniform vec2 uv_scale;
	layout(binding = )SHAD" SS(SHADER_BINDING_DIFFUSE) R"SHAD() uniform sampler2D diffuse;
	out vec4 frag_color;
	void main() {
		float value = texture(diffuse, vec2(uv.x * uv_scale.x, uv.y * uv_scale.y) + uv_offs).r;
		frag_color = vec4(color.xyz, value - (1 - color.w));
	};
)SHAD";
	
static std::unique_ptr<shader::program> basic_prog;
static std::unique_ptr<shader::program> line_prog;
static std::unique_ptr<shader::program> text_prog;

static std::unique_ptr<shader::program> setup_program(char const * name, char const * v_src, char const * f_src) {
	std::string complog;
	
	shader::stage v_shad {GL_VERTEX_SHADER};
	v_shad.source(v_src);
	if (!v_shad.compile(complog)) {
		cilogve("%s vertex shader failed to compile:\n%s", name, complog.c_str());
	}
	
	shader::stage f_shad {GL_FRAGMENT_SHADER};
	f_shad.source(f_src);
	if (!f_shad.compile(complog)) {
		cilogve("%s fragment shader failed to compile:\n%s", name, complog.c_str());
	}
	
	std::unique_ptr<shader::program> prog {new shader::program {}};
	prog->attach(v_shad);
	prog->attach(f_shad);
	if (!prog->link(complog)) {
		cilogve("%s shader program failed to link:\n%s", name, complog.c_str());
	}
	return prog;
}

#define SETUP_SHADER(name) name##_prog = setup_program(#name, name##_v_src, name##_f_src)

void shader::init() {
	SETUP_SHADER(basic);
	SETUP_SHADER(line);
	SETUP_SHADER(text);
}

void shader::term() {
	basic_prog.reset();
	line_prog.reset();
	text_prog.reset();
}

void shader::bind(shader::type t) {
	switch (t) {
		case shader::type::basic:
			glBlendFunc(GL_ONE, GL_ZERO);
			glUseProgram(basic_prog->id);
			break;
		case shader::type::line:
			glBlendFunc(GL_ONE, GL_ZERO);
			glUseProgram(line_prog->id);
			break;
		case shader::type::text:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glUseProgram(text_prog->id);
			break;
		default:
			return;
	}
}

void shader::upload(shader::basic const & b) {
	glUniformMatrix4fv(SHADER_LOCATION_MVP, 1, GL_FALSE, b.m);
	glUniform4f(SHADER_LOCATION_COLOR, b.color[0], b.color[1], b.color[2], b.color[3]);
}

void shader::upload(shader::line const & b) {
	glUniformMatrix4fv(SHADER_LOCATION_MVP, 1, GL_FALSE, b.m);
	glUniform4f(SHADER_LOCATION_COLOR, b.color[0], b.color[1], b.color[2], b.color[3]);
}

void shader::upload(shader::text const & b) {
	glUniformMatrix4fv(SHADER_LOCATION_MVP, 1, GL_FALSE, b.m);
	glUniform4f(SHADER_LOCATION_COLOR, b.color[0], b.color[1], b.color[2], b.color[3]);
	glUniform2f(SHADER_LOCATION_OFFS2D, b.rect[0], b.rect[1]);
	glUniform2f(SHADER_LOCATION_SCALE2D, b.rect[2], b.rect[3]);
	glUniform2f(SHADER_LOCATION_TEXOFFS, b.tex_rect.origin[0], b.tex_rect.origin[1]);
	glUniform2f(SHADER_LOCATION_TEXSCALE, b.tex_rect.extents[0], b.tex_rect.extents[1]);
}
