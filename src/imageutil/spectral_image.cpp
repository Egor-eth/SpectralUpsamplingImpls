#include <utility>
#include <stdexcept>
#include "spectral_image.h"
#include "spectral_util.h"


bool SpectralSaver::operator()(const std::string &path, const Spectre *data, int width, int height) const
{
	if(width == 1 && height == 1) {
		spectral::save_spectre(path, data[0]);
	}
	return true;
}

template class BaseImage<Spectre, void, SpectralSaver>;