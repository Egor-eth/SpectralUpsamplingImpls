#ifndef IMAGEUTIL_PIXEL_H
#define IMAGEUTIL_PIXEL_H
#include <type_traits>
#include <cinttypes>
#include <glm/glm.hpp>

#pragma pack(push, 1)
struct PixelRGBA {
	uint8_t rgba[4];

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

	uint32_t argb() const;

	static PixelRGBA from_argb(uint32_t argb);
	static PixelRGBA from_rgb(uint32_t rgb);
	static PixelRGBA from_vec3(const glm::vec3 &rgb);
	static const PixelRGBA none;
};
#pragma pack(pop)

using Pixel = PixelRGBA;

static_assert(std::is_trivial<Pixel>() && std::is_standard_layout<Pixel>(), "Requires being trivial standard layout object for outputting");

#endif
