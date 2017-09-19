#define RENDER_SOURCE
#include "common.hh"
#include RENDER_PRIVATE

#include "bridge/sdl2_common.hh"
#include "bridge/text.hh"

#include "protoframe.hh"
#include "mesh.hh"
#include "shader.hh"
#include "commandseq.hh"
#include "font.hh"
#include "texture.hh"

using namespace cichlid;

static SDL_GLContext glctx = nullptr;

static render::protoframe * root_frame = nullptr;

size_t render::context_width, render::context_height;

// ================
static bridge::text::instance * console_text_instance = nullptr;
static render::texture::sptr console_text_atlas;

void render::bind_console_text_atlas() { console_text_atlas->bind(SHADER_BINDING_DIFFUSE); }
bridge::text::string render::console_text_generate(std::string const & str) { return console_text_instance->generate(str); }
// ================

void render::init() {
	
	render::context_width = sdl::context_width;
	render::context_height = sdl::context_height;
	
	glctx = SDL_GL_CreateContext(sdl::win);
	gladLoadGLLoader(SDL_GL_GetProcAddress);
	
	SDL_GL_SetSwapInterval(1);
	
	glViewport(0, 0, render::context_width, render::context_height);
	glScissor(0, 0, render::context_width, render::context_height);
	
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glClearColor(0, 0, 0, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLineWidth(1);
	
	texture::init();
	shader::init();
	mesh::init();
	
	console_text_instance = new bridge::text::instance(console_text_size);
	auto const & atlas_img = console_text_instance->get_atlas();
	console_text_atlas = texture::reg(atlas_img.size.width(), atlas_img.size.height(), atlas_img.data);
	
	#define CVAR_REGISTER
	#include "^cvar.inl"
}

void render::term() {
	if (root_frame) delete root_frame;
	root_frame = nullptr;
	
	mesh::term();
	shader::term();
	console_text_atlas.reset();
	if (console_text_instance) delete console_text_instance;
	texture::term();
	
	if (glctx) SDL_GL_DeleteContext(glctx);
	
	glctx = nullptr;
}

void render::preframe() {
	if (root_frame) delete root_frame;
	root_frame = nullptr;
	game::realm const * r = game::get_root_realm();
	if (!r) return;
	root_frame = new protoframe {*r, r->get_default_perspective()};
}

void render::frame() {
	
	if (render::context_width != sdl::context_width || render::context_height != sdl::context_height) {
		render::context_width = sdl::context_width;
		render::context_height = sdl::context_height;
		glViewport(0, 0, render::context_width, render::context_height);
		glScissor(0, 0, render::context_width, render::context_height);
	}
	
	render::command_sequence seq {*root_frame};
	seq.execute();
	
	SDL_GL_SwapWindow(sdl::win);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

std::mutex render::cvar_mutex {};

#define CVAR_DECL
#include "^cvar.inl"
