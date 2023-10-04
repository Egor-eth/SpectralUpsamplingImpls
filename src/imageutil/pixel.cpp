#include "pixel.h"

PixelRGBA::PixelRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	rgba[0] = r;
	rgba[1] = g;
	rgba[2] = b;
	rgba[3] = a;
}

PixelRGBA &PixelRGBA::operator=(uint32_t rgba_in)
{
	rgba[0] = (rgba_in >> 24);
	rgba[1] = (rgba_in >> 16) & 0xFF;
	rgba[2] = (rgba_in >> 8) & 0xFF;
	rgba[3] = rgba_in & 0xFF;
	return *this;
}

PixelRGBA::operator uint32_t() const
{
	return (rgba[0] << 24) | (rgba[1] << 16) | (rgba[2] << 8) | rgba[3];
}

uint32_t PixelRGBA::argb() const
{
	return (rgba[0] << 16) | (rgba[1] << 8) | rgba[2] | (rgba[3] << 24);
}

PixelRGBA PixelRGBA::from_argb(uint32_t argb)
{
	return PixelRGBA(
			(argb >> 16) & 0xFF,
			(argb >> 8) & 0xFF,
			argb & 0xFF,
			argb >> 24
			);
}

PixelRGBA PixelRGBA::from_rgb(uint32_t rgb)
{
	return PixelRGBA(
			(rgb >> 16) & 0xFF,
			(rgb >> 8) & 0xFF,
			(rgb) & 0xFF
			);
}

PixelRGBA PixelRGBA::from_vec3(const glm::vec3 &rgb)
{
	return PixelRGBA(
			static_cast<uint8_t>(rgb.x * 255.999),
			static_cast<uint8_t>(rgb.y * 255.999),
			static_cast<uint8_t>(rgb.z * 255.999)
			);
}

const PixelRGBA PixelRGBA::none{0, 0, 0};
