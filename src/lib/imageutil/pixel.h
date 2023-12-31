#ifndef IMAGEUTIL_PIXEL_H
#define IMAGEUTIL_PIXEL_H
#include <type_traits>
#include <cinttypes>
#include "math/vec3.h"

namespace spec {

	#pragma pack(push, 1)
	union PixelRGBA {
		uint8_t rgba[4];
		struct {
			uint8_t r, g, b, a;
		};
		//required for triviality
		PixelRGBA() = default; // @suppress("Class members should be properly initialized")
		PixelRGBA(const PixelRGBA &) = default;
		PixelRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
		PixelRGBA &operator=(const PixelRGBA&) = default;
		
		PixelRGBA(uint32_t argb) {
			*this = argb;
		}

		PixelRGBA &operator=(uint32_t argb);

		operator uint32_t() const;

		uint8_t operator[](int i) const
		{
			return rgba[i];
		}
		uint8_t &operator[](int i)
		{
			return rgba[i];
		}

		math::vec3 to_vec3() const;
		

		uint32_t argb() const;

		static PixelRGBA from_argb(uint32_t argb);
		static PixelRGBA from_rgb(uint32_t rgb);
		static PixelRGBA from_vec3(const math::vec3 &rgb);
		static const PixelRGBA none;
	};
	#pragma pack(pop)

	using Pixel = PixelRGBA;

	static_assert(std::is_trivial<Pixel>() && std::is_standard_layout<Pixel>(), "Requires being trivial standard layout object for outputting");

}

#endif
