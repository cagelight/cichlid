#include "text.hh"

#include <asterid/bspacker.hh>
#include <algorithm>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

using namespace ci::bridge;

static FT_Library ft2_lib = nullptr;

static constexpr size_t ascii_min = 0x20;
static constexpr size_t ascii_max = 0x7F;

static constexpr size_t raster_padding = 1;

void text::init() {
	if (FT_Init_FreeType(&ft2_lib)) {
		scilogve << "failed to initialize FreeType2";
		throwe(startup);
	}
}

void text::term() noexcept {
	if (ft2_lib) FT_Done_FreeType(ft2_lib);
	ft2_lib = nullptr;
}

text::instance::instance(size_t raster_size) {
	FT_Face face;
	
	FT_New_Face(ft2_lib, "/usr/share/fonts/TTF/DejaVuSansMono.ttf", 0, &face);
	FT_Set_Pixel_Sizes(face, 0, raster_size);

	struct atlas_proto_entry {
		uint8_t c;
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
		i.coords = atlas_proto.pack(i.glyph->bitmap.width + raster_padding, i.glyph->bitmap.rows + raster_padding);
	}
	
	float aw = atlas_proto.width();
	float ah = atlas_proto.height();
	
	characters.resize(ascii_max - ascii_min + 1);
	for (auto & i : proto_entries) {
		character & c = characters[i.c - ascii_min];
		c.ascii = i.c;
		c.transform.x() = static_cast<float>(i.metrics.horiBearingX) / (raster_size * 64);
		c.transform.y() = -(static_cast<float>(i.metrics.height)  - static_cast<float>(i.metrics.horiBearingY)) / (raster_size * 64);
		c.transform.width() = i.glyph->bitmap.width / static_cast<float>(raster_size);
		c.transform.height() = i.glyph->bitmap.rows / static_cast<float>(raster_size);
		c.atlas_uv.x() = static_cast<float>(i.coords.x) / aw;
		c.atlas_uv.y() = 1 - static_cast<float>(i.coords.y + i.glyph->bitmap.rows) / ah;
		c.atlas_uv.width() = i.glyph->bitmap.width / aw;
		c.atlas_uv.height() = i.glyph->bitmap.rows / ah;
		c.advance = static_cast<float>(i.metrics.horiAdvance) / (raster_size * 64);
	}
	
	atlas_img.size.width() = atlas_proto.width();
	atlas_img.size.height() = atlas_proto.height();
	atlas_img.data.resize(atlas_img.size.width() * atlas_img.size.height());
	memset(atlas_img.data.data(), 0, atlas_img.data.size());
	
	for (auto & i : proto_entries) {
		for (unsigned int y = 0; y < i.glyph->bitmap.rows; y++) {
			for (unsigned int x = 0; x < i.glyph->bitmap.width; x++) {
				atlas_img.data.data()[(x + i.coords.x) + (atlas_proto.height() - (y + i.coords.y) - 1) * atlas_proto.width()] = i.glyph->bitmap.buffer[x + y * i.glyph->bitmap.pitch];
			}
		}
		FT_Done_Glyph(reinterpret_cast<FT_Glyph>(i.glyph));
	}
}

text::instance::~instance() {
	
}

text::atlas_image const & text::instance::get_atlas() {
	return atlas_img;
}


static text::character null_char {};
text::character const & text::instance::get_character(uint8_t c) {
	if (c < ascii_min || c > ascii_max) return null_char;
	return characters[c - ascii_min];
}

text::string text::instance::generate(std::string const & str) {
	text::string retr {};
	
	real_t spacing = 0;
	for (char c : str) {
		character const & ch = get_character(c);
		if (!ch.ascii) continue;
		retr.push_back({ vec2_t {spacing, 0}, &ch });
		spacing += ch.advance;
	}
	
	return retr;
}
