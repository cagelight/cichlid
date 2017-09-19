#pragma once

#define RENDER_HEADER
#include "common.hh"
#include RENDER_PUBLIC

// VAO binding
#define SHADER_LOCATION_VERTEX 0 // 3 float
#define SHADER_LOCATION_UV 1 // 2 float
#define SHADER_LOCATION_NORMAL 2 // 3 float
// 3, 4, 5 reserved
// UNIFORMS
#define SHADER_LOCATION_MVP 6 // mat4
#define SHADER_LOCATION_COLOR 7 // vec3
#define SHADER_LOCATION_OFFS2D 8 // vec2 -- positional offset, applied before MVP
#define SHADER_LOCATION_SCALE2D 9 // vec2 -- scale, applied before MVP
#define SHADER_LOCATION_TEXOFFS 10 // vec2 -- texture offset, added to UV
#define SHADER_LOCATION_TEXSCALE 11 // vec2 -- texture UV scale

// BINDINGS
#define SHADER_BINDING_UBUF 0
#define SHADER_BINDING_DIFFUSE 1

namespace cichlid::render::shader {
	
	void init();
	void term();
	
	enum struct type {
		basic,
		line,
		text
	};
	
	struct basic {
		b::mat4_t<float> m {};
		b::vec4_t<float> color {};
	};
	
	struct line {
		b::mat4_t<float> m {};
		b::vec4_t<float> color {};
	};
	
	struct text {
		b::mat4_t<float> m {};
		b::vec4_t<float> color {};
		b::vec4_t<float> rect {};
		b::rect_t<float> tex_rect {};
	};
	
	void bind(type);
	void upload(basic const &);
	void upload(line const &);
	void upload(text const &);
	
};
