#include "game/game.hh"
#include "game/realm.hh"

#include "gproc.hh"

#include "../mesh.hh"
#include "../shader.hh"
#include "../texture.hh"

namespace ci::render::gproc {
	
	static ci::render::texture::sptr raster_fbo_diffuse;
	static ci::render::texture::sptr raster_fbo_depth;
	static ci::render::texture::sptr raster_fbo_position;
	static ci::render::texture::sptr raster_fbo_normals;
	static GLuint raster_fbo = 0;
	
	struct test_cube {
		transformation_t trans;
		vec3_t color;
	};

	struct protoframe {
		bool console_showing;
		bool console_showing_autocomplete;
		std::vector<std::string> console_autocomplete;
		std::string console_string;
		
		std::vector<test_cube> test_cubes;
		
		game::perspective pers;
		
		protoframe(game::realm const & r) {
			pers = r.pers;
			console_showing = game::globals.in_console;
			if (console_showing) {
				console_showing_autocomplete = game::globals.show_autocomplete;
				if (console_showing_autocomplete) console_autocomplete = game::globals.console_autocomplete_options;
				console_string = game::globals.console_text;
			}
			for (auto const & tc : r.test_cubes) {
				test_cubes.push_back( {tc.tC, tc.cC} );
			}
		}
	};

	struct command_sequence {
		
		std::shared_ptr<ci::render::gproc::protoframe> proto;
		mat4_t P;
		mat4_t V;
		mat4_t VP;
		
		command_sequence(std::shared_ptr<ci::render::gproc::protoframe> const & proto) : proto(proto) {
			P = mat4_t::perspective(b::deg2rad<real_t>(r_vfov), context_width, context_height, 0.01, 10000);
			V = /*mat4_t::translate(-proto->pers.pos) * */mat4_t {proto->pers.vec.reciprocal()};
			VP = V * P;
		}
		void draw() {
			
			glBindFramebuffer(GL_FRAMEBUFFER, raster_fbo);
			
			glViewport(0, 0, render::context_width, render::context_height);
			glScissor(0, 0, render::context_width, render::context_height);
			
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glEnable(GL_DEPTH_TEST);
			glBlendFunc(GL_ONE, GL_ZERO);
			
			shader::bind(shader::type::dbasic);
			mesh::bind(mesh::primitive::cube);
			shader::dbasic cube_sh;
			for (auto & tc : proto->test_cubes) {
				tc.trans.pos -= proto->pers.pos;
				cube_sh.m = tc.trans.to_matrix() * VP;
				cube_sh.color = {tc.color, 1};
				shader::upload(cube_sh);
				mesh::draw();
			}
			
			// ================================
			
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			raster_fbo_position->bind(SHADER_BINDING_DIFFUSE);
			shader::bind(shader::type::fbofinish);
			mesh::bind(mesh::primitive::fqzn);
			mesh::draw();
			
			// ================================

			glDisable(GL_DEPTH_TEST);
			if (r_origin_lines) {
				transformation_t line_trans;
				line_trans.pos = -proto->pers.pos;
				line_trans.scale = {100000000};
				mat4_t line_mat = line_trans.to_matrix() * VP;
				shader::bind(shader::type::line);
				glBlendFunc(GL_ONE, GL_ZERO);
				mesh::bind(mesh::primitive::line_x);
				shader::upload(shader::line {line_mat, {1, 0, 0, 1}});
				mesh::draw();
				mesh::bind(mesh::primitive::line_y);
				shader::upload(shader::line {line_mat, {0, 1, 0, 1}});
				mesh::draw();
				mesh::bind(mesh::primitive::line_z);
				shader::upload(shader::line {line_mat, {0, 0, 1, 1}});
				mesh::draw();
			}
			
			// ================================
			// quick trash console code, will make something better later
			// ================================
			if (proto->console_showing) {
				mesh::bind(mesh::primitive::uqzn);
				shader::bind(shader::type::basic);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
				
				if (proto->console_showing_autocomplete) {
					console_text_trans.pos = {0, entry_height, 0};
					console_bg.color = {0.125, 0.125, 0.125, 0.75};
					size_t auto_lines = proto->console_autocomplete.size();
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
				bind_ctext_atlas();
				
				console_text_trans.scale = {console_text_size};
				
				if (proto->console_showing_autocomplete) {
					size_t auto_lines = proto->console_autocomplete.size();
					for (size_t i = 0; i < auto_lines; i++) {
						console_text_trans.pos = {padding, padding + (console_text_size + line_spacing) * i + text_offset + entry_height, 0};
						auto str = render::ctext_i->generate(proto->console_autocomplete[i]);
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
				auto str = render::ctext_i->generate("> " + proto->console_string);
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
					auto str = render::ctext_i->generate(*log_e);
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
			
			glDisable(GL_DEPTH_TEST);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			mesh::bind(mesh::primitive::uqzn);
			shader::bind(shader::type::text);
			bind_ctext_atlas();
			transformation_t fstat_trans;
			fstat_trans.pos = {3, static_cast<double>(context_height - fstat_text_size - 3), 0};
			fstat_trans.scale = {fstat_text_size, fstat_text_size, 1};
			b::mat4_t<float> fstat_proj = b::mat4_t<float>::ortho(context_height, 0, 0, context_width, -100, 100);
			shader::text fstat_fps_sh;
			fstat_fps_sh.color = {0, 1, 0.5, 1};
			fstat_fps_sh.m = fstat_trans.to_matrix() * fstat_proj;
			std::stringstream fstat_ss;
			fstat_ss << std::fixed << std::setprecision(2);
			fstat_ss << "LAST: " << render::fstat.last_fps() << "  ";
			fstat_ss << "SMOOTH: " << render::fstat.smooth_fps() << "  ";
			fstat_ss << "LOW: " << render::fstat.low_fps() << "  ";
			fstat_ss << "HIGH: " << render::fstat.high_fps() << "  ";
			auto str = render::ctext_i->generate(fstat_ss.str());
			for (auto const & pc : str) {
				fstat_fps_sh.rect = pc.c->transform.offsetted(pc.pos);
				fstat_fps_sh.tex_rect = pc.c->atlas_uv;
				shader::upload(fstat_fps_sh);
				mesh::draw();
			}
		}
	};
}

static std::shared_ptr<ci::render::gproc::protoframe> proto;
static std::shared_ptr<ci::render::gproc::command_sequence> cmdseq;

void ci::render::gproc::generate_proto() {
	cmdseq.reset();
	game::realm const & root_realm = game::get_realm();
	proto.reset(new protoframe {root_realm});
}

void ci::render::gproc::generate_cmdseq() {
	cmdseq.reset(new command_sequence {proto});
}

void ci::render::gproc::draw() {
	cmdseq->draw();
}

void ci::render::gproc::init() {
	
	glCreateFramebuffers(1, &raster_fbo);
	
	resize_buffers();
	
	GLenum raster_draws [] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glNamedFramebufferDrawBuffers(raster_fbo, 3, raster_draws);
	
	if(glCheckNamedFramebufferStatus(raster_fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		scilogve << "raster FBO is not complete: " << glCheckNamedFramebufferStatus(raster_fbo, GL_FRAMEBUFFER);
		throwe(startup);
	}
}

void ci::render::gproc::term() noexcept {
	raster_fbo_diffuse.reset();
	raster_fbo_depth.reset();
	raster_fbo_position.reset();
	raster_fbo_normals.reset();
	if (raster_fbo) {
		glDeleteFramebuffers(1, &raster_fbo);
		raster_fbo = 0;
	}
}

void ci::render::gproc::resize_buffers() {
	if (raster_fbo_diffuse) texture::markfordelete(raster_fbo_diffuse);
	if (raster_fbo_depth) texture::markfordelete(raster_fbo_depth);
	if (raster_fbo_position) texture::markfordelete(raster_fbo_position);
	if (raster_fbo_normals) texture::markfordelete(raster_fbo_normals);
	
	raster_fbo_diffuse = texture::reg(context_width, context_height, GL_RGBA32F);
	raster_fbo_depth = texture::reg(context_width, context_height, GL_DEPTH_COMPONENT32);
	raster_fbo_position = texture::reg(context_width, context_height, GL_RGBA32F);
	raster_fbo_normals = texture::reg(context_width, context_height, GL_RGBA32F);
	
	glTextureParameteri(raster_fbo_diffuse->get(), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(raster_fbo_diffuse->get(), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(raster_fbo_depth->get(), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(raster_fbo_depth->get(), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(raster_fbo_position->get(), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(raster_fbo_position->get(), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(raster_fbo_normals->get(), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(raster_fbo_normals->get(), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glNamedFramebufferTexture(raster_fbo, GL_COLOR_ATTACHMENT0, raster_fbo_diffuse->get(), 0);
	glNamedFramebufferTexture(raster_fbo, GL_DEPTH_ATTACHMENT, raster_fbo_depth->get(), 0);
	glNamedFramebufferTexture(raster_fbo, GL_COLOR_ATTACHMENT1, raster_fbo_position->get(), 0);
	glNamedFramebufferTexture(raster_fbo, GL_COLOR_ATTACHMENT2, raster_fbo_normals->get(), 0);
}
