#define RENDER_SOURCE
#include "common.hh"
#include RENDER_PRIVATE
#include GAME_PUBLIC

#include "commandseq.hh"

using namespace cichlid;

render::command_sequence::command_sequence(protoframe const & proto) {
	
	mat4_t perspective = mat4_t::perspective(b::deg2rad<real_t>(r_vfov), context_width, context_height, 0.001, 100000);
	mat4_t view = mat4_t::translate(-proto.pers.pos) * mat4_t {proto.pers.vec.reciprocal()};
	
	if (r_origin_lines) {
		origin_lines.m_x = mat4_t::scale(vec3_t {100000000}) * view * perspective;
		origin_lines.m_y = mat4_t::scale(vec3_t {100000000}) * view * perspective;
		origin_lines.m_z = mat4_t::scale(vec3_t {100000000}) * view * perspective;
	}
	
	for (object::cube const & c : proto.cubes) {
		mat4_t model = c.trans.to_matrix();
		mat4_t mvp = model * view * perspective;
		this->basic_seq.push_back( { b::mat4_t<float> {mvp}, {c.color, 1} } );
	}
	
	console_showing = game::globals.in_console;
	if (console_showing) {
		console_showing_autocomplete = game::globals.show_autocomplete;
		if (console_showing_autocomplete) console_autocomplete = game::globals.console_autocomplete_options;
		console_string = game::globals.console_text;
	}
}

void render::command_sequence::execute() {
	shader::bind(shader::type::basic);
	mesh::bind(mesh::primitive::cube);
	for (shader::basic const & b : basic_seq) {
		shader::upload(b);
		mesh::draw();
	}
	
	glDisable(GL_DEPTH_TEST);
	if (r_origin_lines) {
		shader::bind(shader::type::line);
		mesh::bind(mesh::primitive::line_x);
		shader::upload(shader::line {origin_lines.m_x, {1, 0, 0, 1}});
		mesh::draw();
		mesh::bind(mesh::primitive::line_y);
		shader::upload(shader::line {origin_lines.m_y, {0, 1, 0, 1}});
		mesh::draw();
		mesh::bind(mesh::primitive::line_z);
		shader::upload(shader::line {origin_lines.m_z, {0, 0, 1, 1}});
		mesh::draw();
	}
	
	// ================================
	// quick trash console code, will make something better later
	// ================================
	if (console_showing) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		mesh::bind(mesh::primitive::uqzn);
		transformation_t console_text_trans;
		b::mat4_t<float> c_proj = b::mat4_t<float>::ortho(context_height, 0, 0, context_width, -100, 100);
		
		size_t console_line_count = (context_height / (console_text_size+2)) / 3;
		double padding = 4;
		double text_offset = 4;
		double line_spacing = 2;
		
		shader::basic console_bg;
		console_text_trans.pos = {0, static_cast<double>(context_height), 0};
		console_text_trans.scale = {static_cast<double>(context_width), -static_cast<double>((console_text_size + line_spacing) * console_line_count + padding), 1};
		
		console_bg.color = {0.25, 0.25, 0.25, 0.75};
		console_bg.m = console_text_trans.to_matrix() * c_proj;
		shader::upload(console_bg);
		mesh::draw();
		
		double entry_height = static_cast<double>(console_text_size + text_offset + padding * 2);
		
		if (console_showing_autocomplete) {
			console_text_trans.pos = {0, entry_height, 0};
			console_bg.color = {0.125, 0.125, 0.125, 0.75};
			size_t auto_lines = console_autocomplete.size();
			console_text_trans.scale = {static_cast<double>(context_width) / 2, static_cast<double>((console_text_size + line_spacing) * auto_lines + text_offset + padding*2), 1};
			console_bg.m = console_text_trans.to_matrix() * c_proj;
			shader::upload(console_bg);
			mesh::draw();
		}
		
		console_text_trans.pos = {0};
		console_text_trans.scale = {static_cast<double>(context_width), entry_height, 1};
		console_bg.color = {0.25, 0.25, 0.25, 0.75};
		console_bg.m = console_text_trans.to_matrix() * c_proj;
		shader::upload(console_bg);
		mesh::draw();
		
		console_text_trans = {};
		shader::bind(shader::type::text);
		mesh::bind(mesh::primitive::uqzn);
		render::bind_console_text_atlas();
		
		console_text_trans.scale = {console_text_size};
		
		if (console_showing_autocomplete) {
			size_t auto_lines = console_autocomplete.size();
			for (size_t i = 0; i < auto_lines; i++) {
				console_text_trans.pos = {padding, padding + (console_text_size + line_spacing) * i + text_offset + entry_height, 0};
				auto str = render::console_text_generate(console_autocomplete[i]);
				for (auto const & pc : str) {
					shader::text uni;
					uni.color = {0, 1, 0.5, 1};
					uni.m = console_text_trans.to_matrix() * c_proj;
					uni.rect = pc.c->transform.offsetted(pc.pos);
					uni.tex_rect = pc.c->atlas_uv;
					shader::upload(uni);
					mesh::draw();
				}
			}
		}
		
		console_text_trans.pos = {padding, padding + text_offset, 0};
		auto str = render::console_text_generate("> " + console_string);
		for (auto const & pc : str) {
			shader::text uni;
			uni.color = {0, 1, 0.5, 1};
			uni.m = console_text_trans.to_matrix() * c_proj;
			uni.rect = pc.c->transform.offsetted(pc.pos);
			uni.tex_rect = pc.c->atlas_uv;
			shader::upload(uni);
			mesh::draw();
		}
		
		auto log = com::cmd.log(console_line_count);
		for (size_t i = 0; i < console_line_count; i++) {
			auto log_e = log.begin() + i;
			if (log_e == log.end()) break;
			auto str = render::console_text_generate(*log_e);
			console_text_trans.pos = {padding, static_cast<double>(context_height - (console_text_size + line_spacing) * (console_line_count - i) + text_offset - padding), 0};
			for (auto const & pc : str) {
				shader::text uni;
				uni.color = {0, 1, 0.5, 1};
				uni.m = console_text_trans.to_matrix() * c_proj;
				uni.rect = pc.c->transform.offsetted(pc.pos);
				uni.tex_rect = pc.c->atlas_uv;
				shader::upload(uni);
				mesh::draw();
			}
		}
	}
	// ================================
	
	glEnable(GL_DEPTH_TEST);
}
