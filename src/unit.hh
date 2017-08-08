#pragma once

#include "common.hh"

namespace cichlid {
	
	typedef double length_t;
	
	typedef b::vec3_t<length_t> vec3_t;
	typedef b::vec3_t<length_t> scale_t;
	typedef b::quaternion_t<length_t> quat_t;
	
	typedef b::mat4_t<length_t> mat4_t;
	
	struct transformation_t {
		vec3_t pos {};
		quat_t rot {};
		scale_t scale {1};
		inline b::mat4_t<length_t> to_matrix() const {
			return mat4_t::scale(scale) * mat4_t(rot) * mat4_t::translate(pos);
		}
	};
	
}
