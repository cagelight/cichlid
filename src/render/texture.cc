#include "texture.hh"

#include <vector>
#include <algorithm>

static std::vector<std::shared_ptr<ci::render::texture::cache>> texture_cache {};

void ci::render::texture::init() {
	
}

void ci::render::texture::term() {
	texture_cache.clear();
}

GLuint ci::render::texture::cache::get() {
	_last_access = as::time::now<as::time::clock_type::monotonic>();
	return _id;
}

void ci::render::texture::cache::bind(GLuint unit) {
	_last_access = as::time::now<as::time::clock_type::monotonic>();
	glBindTextureUnit(unit, _id);
}

std::vector<ci::render::texture::sptr> texture_store {}; // FIXME -- very bad

ci::render::texture::sptr ci::render::texture::reg(uint_fast16_t width, uint_fast16_t height, GLuint format) {
	sptr ptr {nullptr};
	
	GLuint tex_id = 0;
	glCreateTextures(GL_TEXTURE_2D, 1, &tex_id);
	glTextureStorage2D(tex_id, 1, format, width, height);
	
	ptr.reset(new cache {tex_id});
	texture_store.push_back(ptr);
	
	return ptr;
}

ci::render::texture::sptr ci::render::texture::reg(uint_fast16_t width, uint_fast16_t height, as::buffer_assembly const & data, GLuint format, GLuint data_layout, GLuint data_type) {
	sptr ptr {nullptr};
	
	GLuint tex_id = 0;
	glCreateTextures(GL_TEXTURE_2D, 1, &tex_id);
	glTextureStorage2D(tex_id, 1, format, width, height);
	glTextureSubImage2D(tex_id, 0, 0, 0, width, height, data_layout, data_type, data.data());
	
	ptr.reset(new cache {tex_id});
	texture_store.push_back(ptr);
	
	return ptr;
}

void ci::render::texture::markfordelete(sptr ptr) {
	texture_store.erase(std::remove(texture_store.begin(), texture_store.end(), ptr));
}
