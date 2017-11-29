#include "bridge/sdl2_common.hh"
#include "bridge/text.hh"
#include "render_private.hh"

#include "mesh.hh"
#include "shader.hh"
#include "texture.hh"

#include "gproc/gproc.hh"

size_t ci::render::context_width;
size_t ci::render::context_height;
static SDL_GLContext glctx = nullptr;

std::unique_ptr<ci::bridge::text::instance> ci::render::ctext_i;
static ci::render::texture::sptr ctext_atlas;
void ci::render::bind_ctext_atlas() { ctext_atlas->bind(SHADER_BINDING_DIFFUSE); }

#define CVAR_DECL
#include "xcvar.inl"

static as::time::keeper<as::time::clock_type::monotonic> rtk;

void ci::render::init() {
	render::context_width = sdl2::context_width;
	render::context_height = sdl2::context_height;
	
	glctx = SDL_GL_CreateContext(sdl2::win);
	gladLoadGLLoader(SDL_GL_GetProcAddress);
	
	SDL_GL_SetSwapInterval(1);
	
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
	gproc::init();
	
	ctext_i.reset(new bridge::text::instance {console_text_size});
	auto const & ctext_atlas_raw = ctext_i->get_atlas();
	ctext_atlas = texture::reg(ctext_atlas_raw.size.width(), ctext_atlas_raw.size.height(), ctext_atlas_raw.data, GL_R8, GL_RED, GL_UNSIGNED_BYTE);
	
	#define CVAR_REGISTER
	#include "xcvar.inl"
	
	rtk.mark();
}

void ci::render::term() noexcept {
	gproc::term();
	mesh::term();
	shader::term();
	ctext_atlas.reset();
	texture::term();
	
	ctext_i.reset();
	
	if (glctx) SDL_GL_DeleteContext(glctx);
	glctx = nullptr;
}

void ci::render::preframe() {
	gproc::generate_proto();
}

static ci::render::frame_statistics fstat_;
ci::render::frame_statistics const & ci::render::fstat {fstat_};

void ci::render::frame() {
	if (render::context_width != sdl2::context_width || render::context_height != sdl2::context_height) {
		render::context_width = sdl2::context_width;
		render::context_height = sdl2::context_height;
		gproc::resize_buffers();
	}
	
	glViewport(0, 0, render::context_width, render::context_height);
	glScissor(0, 0, render::context_width, render::context_height);
	
	gproc::generate_cmdseq();
	gproc::draw();
	
	SDL_GL_SwapWindow(sdl2::win);
	auto ts = rtk.mark();
	fstat_.mark(ts.sec());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
