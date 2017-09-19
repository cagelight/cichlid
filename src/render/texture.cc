#define RENDER_SOURCE
#include "common.hh"
#include RENDER_PRIVATE

#include "texture.hh"
#include <vector>
#include <algorithm>

using namespace cichlid;

static std::vector<std::shared_ptr<render::texture::cache>> texture_cache {};

void render::texture::init() {
	
}

void render::texture::term() {
	texture_cache.clear();
}

GLuint render::texture::cache::get() {
	_last_access = as::time::now<as::time::clock_type::monotonic>();
	return _id;
}

void render::texture::cache::bind(GLuint unit) {
	_last_access = as::time::now<as::time::clock_type::monotonic>();
	glBindTextureUnit(unit, _id);
}

std::vector<render::texture::sptr> texture_store {}; // FIXME -- very bad

render::texture::sptr render::texture::reg(uint_fast16_t width, uint_fast16_t height, as::byte_buffer const & data) {
	sptr ptr {nullptr};
	
	GLuint tex_id = 0;
	glCreateTextures(GL_TEXTURE_2D, 1, &tex_id);
	glTextureStorage2D(tex_id, 1, GL_R8, width, height);
	glTextureSubImage2D(tex_id, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, data.data());
	
	ptr.reset(new cache {tex_id});
	texture_store.push_back(ptr);
	
	return ptr;
}

void render::texture::markfordelete(sptr ptr) {
	texture_store.erase(std::remove(texture_store.begin(), texture_store.end(), ptr));
}
