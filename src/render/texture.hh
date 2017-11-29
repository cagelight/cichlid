#pragma once

#include "render_private.hh"

#include <asterid/buffer_assembly.hh>

namespace ci::render::texture {
	
	void init();
	void term();
	
	struct cache {
		inline cache(GLuint id) : _id(id) {}
		inline ~cache() { if (_id) glDeleteTextures(1, &_id); }
		GLuint get();
		void bind(GLuint unit);
		inline as::time::point const & last_access() { return _last_access; }
	private:
		GLuint _id;
		as::time::point _last_access;
	};
	
	typedef std::shared_ptr<cache> sptr;
	typedef std::weak_ptr<cache> wptr;
	
	sptr reg(uint_fast16_t width, uint_fast16_t height, GLuint format);
	sptr reg(uint_fast16_t width, uint_fast16_t height, as::buffer_assembly const & data, GLuint format, GLuint data_layout, GLuint data_type); /* GL_R8, GL_RED, GL_UNSIGNED_BYTE */
	
	void markfordelete(sptr);
	
}
