#pragma once

/*
#include "render_internal.hh"

#include <vector>

namespace cichlid::render::font {
	
	void init();
	void term();
	
	struct proto {
		b::rect_t<float> rect;
		b::rect_t<float> uv;
	};
	
	struct cache {
		cache(size_t size); // default font face
		~cache();
		inline as::time::point const & last_access() { return _last_access; }
		void bind_atlas(GLuint unit);
		std::vector<proto> generate(char const *) const;
	private:
		struct private_data;
		std::unique_ptr<private_data> data;
		as::time::point _last_access;
	};
	
	typedef std::shared_ptr<cache> sptr;
	typedef std::weak_ptr<cache> wptr;
	
	sptr reg(size_t raster_size);
	
}
*/
