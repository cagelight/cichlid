#pragma once

#include "render.hh"
#include "bridge/sdl2_common.hh"
#include "bridge/text.hh"

namespace ci::render {
	
	extern size_t context_width, context_height;
	
	static constexpr size_t console_text_size = 10;
	static constexpr size_t fstat_text_size = 10;
	
	extern std::unique_ptr<bridge::text::instance> ctext_i;
	void bind_ctext_atlas();
	
	struct frame_statistics {
		frame_statistics() {
			for (size_t i = 0; i < smooth_buffer_size; i++) smooth_buffer[i] = 0;
		}
		inline void mark(double impulse) {
			last_fps_ = 1 / impulse;
			smooth_buffer[smooth_buffer_index++] = impulse;
			if (smooth_buffer_index >= smooth_buffer_size) smooth_buffer_index = 0;
		}
		
		inline double last_fps() const { return last_fps_; }
		inline double smooth_fps() const { double v = 0; for (size_t i = 0; i < smooth_buffer_size; i++) v += smooth_buffer[i]; return smooth_buffer_size / v; }
		inline double low_fps() const { double v = smooth_buffer[0]; for (size_t i = 1; i < smooth_buffer_size; i++) if (smooth_buffer[i] > v) v = smooth_buffer[i]; return 1 / v; }
		inline double high_fps() const { double v = smooth_buffer[0]; for (size_t i = 1; i < smooth_buffer_size; i++) if (smooth_buffer[i] < v) v = smooth_buffer[i]; return 1 / v; }
	protected:
		static constexpr size_t smooth_buffer_size = 60;
		double last_fps_ = 0;
		double smooth_buffer [smooth_buffer_size];
		size_t smooth_buffer_index = 0;
	};
	
	extern frame_statistics const & fstat;
	
	#define CVAR_EXTERNDECL
	#include "xcvar.inl"
}
