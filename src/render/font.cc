#include <asterid/unicode.hh>
#include <asterid/bspacker.hh>

/*
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <unicode/ustring.h>

#include <vector>
#include <unordered_map>
#include <algorithm>

static FT_Library ft2_lib = nullptr;

namespace cichlid::render::font {
	
	static constexpr size_t ascii_min = 0x20;
	static constexpr size_t ascii_max = 0x7F;
	
	struct cache::private_data {
		struct atlas_entry {
			b::rect_t<float> trans_rect;
			b::rect_t<float> atlas_rect;
			b::vec2_t<float> advance;
		};
		struct scatter_entry {
			b::rect_t<float> trans_rect;
			b::vec2_t<float> advance;
			render::texture::sptr tex {};
		};
		private_data(size_t rs) : raster_size(rs) {
			
			FT_New_Face(ft2_lib, "/usr/share/fonts/TTF/DejaVuSansMono.ttf", 0, &face);
			FT_Set_Pixel_Sizes(face, 0, raster_size);
			
			struct atlas_proto_entry {
				as::unicode::uchar c;
				FT_BitmapGlyph glyph;
				FT_Glyph_Metrics metrics;
				as::bspacker<size_t>::bsp_coords coords;
			};
			
			std::vector<atlas_proto_entry> proto_entries {};
			
			for (size_t i = ascii_min; i <= ascii_max; i++) {
				atlas_proto_entry ape {};
				ape.c = i;
				auto cindex = FT_Get_Char_Index(face, i);
				FT_Load_Glyph(face, cindex, FT_LOAD_RENDER);
				FT_Get_Glyph(face->glyph, reinterpret_cast<FT_Glyph *>(&ape.glyph));
				ape.metrics = face->glyph->metrics;
				proto_entries.push_back(ape);
			}
			
			std::sort(proto_entries.begin(), proto_entries.end(), [](atlas_proto_entry const & a, atlas_proto_entry const & b){return a.glyph->bitmap.rows * a.glyph->bitmap.width > b.glyph->bitmap.rows * b.glyph->bitmap.rows;});
			as::bspacker<size_t> atlas_proto {0, 0};
			
			for (auto & i : proto_entries) {
				i.coords = atlas_proto.pack(i.glyph->bitmap.width, i.glyph->bitmap.rows);
			}
			
			float aw = atlas_proto.width();
			float ah = atlas_proto.height();
			
			atlas_ascii.resize(ascii_max - ascii_min + 1);
			
			for (auto & i : proto_entries) {
				atlas_entry & ent = atlas_ascii[i.c - ascii_min];
				ent.trans_rect.x() = static_cast<float>(i.metrics.horiBearingX) / (raster_size * 64);
				ent.trans_rect.y() = -(static_cast<float>(i.metrics.height)  - static_cast<float>(i.metrics.horiBearingY)) / (raster_size * 64);
				ent.trans_rect.width() = i.glyph->bitmap.width / static_cast<float>(raster_size);
				ent.trans_rect.height() = i.glyph->bitmap.rows / static_cast<float>(raster_size);
				ent.atlas_rect.x() = static_cast<float>(i.coords.x) / aw;
				ent.atlas_rect.y() = 1 - static_cast<float>(i.coords.y + i.glyph->bitmap.rows) / ah;
				ent.atlas_rect.width() = i.glyph->bitmap.width / aw;
				ent.atlas_rect.height() = i.glyph->bitmap.rows / ah;
				ent.advance = { static_cast<float>(i.metrics.horiAdvance) / (raster_size * 64), static_cast<float>(i.metrics.vertAdvance) / (raster_size * 64) };
			}
			
			as::byte_buffer data {};
			data.resize(atlas_proto.width() * atlas_proto.height());
			memset(data.data(), 0x80, data.size());
			
			for (auto & i : proto_entries) {
				for (unsigned int y = 0; y < i.glyph->bitmap.rows; y++) {
					for (unsigned int x = 0; x < i.glyph->bitmap.width; x++) {
						data.data()[(x + i.coords.x) + (atlas_proto.height() - (y + i.coords.y) - 1) * atlas_proto.width()] = i.glyph->bitmap.buffer[x + y * i.glyph->bitmap.pitch];
					}
				}
			}
		
			for (auto & i : proto_entries) {
				FT_Done_Glyph(reinterpret_cast<FT_Glyph>(i.glyph));
			}
			
			atlas = texture::reg(atlas_proto.width(), atlas_proto.height(), data);
		}
		~private_data() {
			if (face) FT_Done_Face(face);
			render::texture::markfordelete(atlas);
		}
		FT_Face face = nullptr;
		size_t raster_size;
		std::vector<atlas_entry> atlas_ascii {};
		std::unordered_map<as::unicode::uchar, scatter_entry> scatter_map {};
		render::texture::sptr atlas {};
	};
}

using namespace cichlid;


//static FT_Face default_face = nullptr;
//static render::font::font_cache default_font;

static std::vector<render::font::sptr> loaded_fonts {};

render::font::cache::cache(size_t rs) {
	data.reset(new private_data {rs});
}

render::font::cache::~cache() {}

void render::font::cache::bind_atlas(GLuint unit) {
	data->atlas->bind(unit);
}

std::vector<render::font::proto> render::font::cache::generate(char const * text) const {
	std::vector<proto> retr {};
	
	float spacing = 0;
	for (size_t i = 0; i < strlen(text); i++) {
		char32_t c = text[i];
		proto p {};
		
		if (c < ascii_min || c > ascii_max) continue;
		private_data::atlas_entry const & ae = data->atlas_ascii[c - ascii_min];
		p.rect = {spacing + ae.trans_rect.x(), ae.trans_rect.y(), ae.trans_rect.width(), ae.trans_rect.height()};
		p.uv = ae.atlas_rect;
		spacing += ae.advance.x();
		
		retr.push_back(p);
	}
	return retr;
}

void render::font::init() {
	if (FT_Init_FreeType(&ft2_lib)) {
		cilogve("failed to initialize FreeType2");
		throwe(startup);
	}
}

void render::font::term() {
	if (ft2_lib) FT_Done_FreeType(ft2_lib);
	ft2_lib = nullptr;
}

render::font::sptr render::font::reg(size_t raster_size) {
	return sptr { new cache {raster_size} };
}
*/
