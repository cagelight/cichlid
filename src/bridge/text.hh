#pragma once

#define BRIDGE_HEADER
#include "common.hh"
#include BRIDGE_PUBLIC

/**
 * This should be a temporary text system, just something basic that can do ASCII.
 * I'll mess with all that fontconfig and international crap later...
 */

namespace cichlid::bridge::text {
	
	void init();
	void term() noexcept;
	
	/**
	 * single channel, 8-bit per pixel
	 */
	struct atlas_image {
		as::byte_buffer data;
		rect_t size;
	};
	
	struct character {
		uint8_t ascii = 0;
		rect_t transform;
		rect_t atlas_uv;
		real_t advance;
		inline bool is_drawable() const {
			return atlas_uv.width() && atlas_uv.height();
		}
	};
	
	struct placed_character {
		vec2_t pos;
		character const * c;
	};
	
	typedef std::vector<placed_character> string;
	
	struct instance {
		instance(size_t size);
		~instance();
		
		atlas_image const & get_atlas();
		character const & get_character(uint8_t);
		string generate(std::string const & str);
	private:
		std::vector<text::character> characters;
		atlas_image atlas_img;
	};
	
}
