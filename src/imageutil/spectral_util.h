#ifndef IMAGEUTIL_SPECTRAL_UTIL_H
#define IMAGEUTIL_SPECTRAL_UTIL_H
#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include "spectre.h"
#include "spectral_image.h"

namespace spectral
{

	struct SavingContext
	{
		const Spectre *data;
		int width;
		int height;
	};

	struct SavingResult
	{
		bool success;
		int channels_used;
		SpectreFloat norm_min;
		SpectreFloat norm_range;
	};

	struct MetadataEntry
	{
		std::string path;
		std::vector<SpectreFloat> targets;
		SpectreFloat norm_min_val;
		SpectreFloat norm_range;
	};

	struct Metadata
	{
		int width;
		int height;
		std::string format;
		std::vector<MetadataEntry> wavelenghts;

		void save(std::ostream &stream) const;
		void load(std::istream &stream);
	};

	void save_spectre(const std::string &path, const Spectre &spectre);

	/**
	 * 	Saves specified wavelenghts of spectral image in multichannel png file (up to 4 channels).
	 * Writes saving data to res. If wavelenghts is empty or contains more than 4 
	 * elements exception is thrown.
	 */
	void save_png_multichannel(std::ostream &stream, const SavingContext &ctx, const std::vector<SpectreFloat> &wavelenghts, SavingResult &res, int requested_channels = 0);

	void save_png_1(std::ostream &stream, const SavingContext &ctx, SpectreFloat wavelenght, SavingResult &res);


}

#endif