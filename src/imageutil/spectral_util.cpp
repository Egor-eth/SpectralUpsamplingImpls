#include "spectral_util.h"
#include <fstream>
#include <stdexcept>
#include <unordered_set>
#include <format>
#include <memory>
#include <limits>
#include <stb_image_write.h>

namespace {
	void normalize_and_convert_to_rgb(const spectral::SavingContext &ctx, unsigned char *dst, const std::vector<SpectreFloat> &wavelenghts, int channels, SpectreFloat &range_out, SpectreFloat &min_val_out)
	{

		//calculate normalization data
		SpectreFloat min_val = std::numeric_limits<SpectreFloat>::max();
		SpectreFloat max_val = std::numeric_limits<SpectreFloat>::min();

		const Spectre *ptr;
		const Spectre *end = ctx.data + ctx.height * ctx.width;
		for(ptr = ctx.data; ptr < end; ++ptr) {
			for(SpectreFloat w : wavelenghts) {
				SpectreFloat val = (*ptr)[w];
				if(val > max_val) max_val = val; 
				if(val < min_val) min_val = val;
			}
		}

		SpectreFloat range = max_val - min_val;
		if(range < 1.0f) { //what about small values?
			range = 1.0f;
		}

		//normalize and write to rgb buffer
		const int vector_size = wavelenghts.size();
		for(long i = 0; i < ctx.height * ctx.width; ++i) {
			for(int c = 0; c < vector_size; ++c) {
				SpectreFloat w = ctx.data[i][wavelenghts[c]];
				SpectreFloat w_norm = (w - min_val) / range;
				dst[i * channels + c] = static_cast<unsigned char>(w_norm * 255.999f);
			}
			for(int c = wavelenghts.size(); c < channels; ++c) {
				dst[i * channels + c] = 0; //zero additional channels
			}
		}

		range_out = range;
		min_val_out = min_val;
	}
}


namespace spectral {
	void save_spectre(const std::string &path, const Spectre &spectre)
	{
		std::ofstream file(path, std::ios::trunc);
		if(!file) throw std::runtime_error("Cannot open file");

		for(const auto &e : spectre.get_map()) {
			file << std::format("%.2f %.2f\n", e.first, e.second);
		}

		file.flush();
	}

	void save_png_multichannel(const SavingContext &ctx, const std::vector<SpectreFloat> &wavelenghts, SavingResult &res, int requested_channels)
	{
		const int vector_size = wavelenghts.size();
		if(vector_size < 1 || vector_size > 4) throw std::invalid_argument("Illegal wavelenghts size");

		int channels = vector_size > requested_channels ? vector_size : requested_channels;

		unsigned char *buf = new unsigned char[ctx.width * ctx.height * channels];
		if(buf == nullptr) throw std::bad_alloc();


		normalize_and_convert_to_rgb(ctx, buf, wavelenghts, channels, res.norm_range, res.norm_min);

		int code = stbi_write_png(ctx.path.c_str(), ctx.width, ctx.height, channels, buf, 0);
		delete[] buf;
		if(!code) {
			throw std::runtime_error("Error saving to file");
		}
		res.channels_used = channels;
	}

	void save_png_1(const SavingContext &ctx, SpectreFloat wavelenght, SavingResult *res)
	{
		(void) ctx;
		(void) wavelenght;
		(void) res;
	}
}