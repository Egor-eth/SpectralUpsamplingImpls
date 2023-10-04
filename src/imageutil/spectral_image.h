#ifndef IMAGEUTIL_SPECTRAL_IMAGE_H
#define IMAGEUTIL_SPECTRAL_IMAGE_H
#include "base_image.h"
#include "constants.h"

using SpectreFloat = float;

class Spectre
{
public: 
	Spectre()
		: spectre() {}

	Spectre(const Spectre &s)
	{
		std::copy(s.spectre, s.spectre + SPECTRE_LENGTH, spectre);
	}

	SpectreFloat &operator[](int p)
	{
		return spectre[p];
	}

	SpectreFloat operator[](int p) const
	{
		return spectre[p];
	}

	Spectre &operator=(const Spectre &other)
	{
		if(&other != this) {
			
		}
		return *this;
	}

	static const Spectre none;

private:
	SpectreFloat spectre[SPECTRE_LENGTH];
};

using SpectralImage = BaseImage<Spectre>; 

#endif