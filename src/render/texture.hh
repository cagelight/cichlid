#pragma once

#define RENDER_HEADER
#include "common.hh"
#include RENDER_PRIVATE

#include <asterid/byte_buffer.hh>

#include <memory>

namespace cichlid::render::texture {
	
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
	
	sptr reg(uint_fast16_t width, uint_fast16_t height, as::byte_buffer const & data);
	void markfordelete(sptr);
	
}
