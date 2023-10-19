#include "spectral_util.h"
#include <fstream>
#include <stdexcept>
#include <unordered_set>
#include <format>
#include <memory>
#include <limits>
#include <stb_image_write.h>
#include <nlohmannjson/json.hpp>

namespace
{

	void __to_stream(void *context, void *data, int size)
	{

		std::ostream *stream = reinterpret_cast<std::ostream *>(context);
		stream->write(reinterpret_cast<const char *>(data), size);
		stream->flush();
	}

	int write_png_to_stream(std::ostream &stream, int width, int height, int channels, const unsigned char *buf)
	{
		return stbi_write_png_to_func(__to_stream, reinterpret_cast<void *>(&stream), width, height, channels, buf, 0);
	}

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

using json = nlohmann::json;

namespace spectral
{

	void Metadata::save(std::ostream &stream) const
	{
		json meta;
		meta["width"] = width;
		meta["height"] = height;

		meta["format"] = format;

		json wavelenghts_array = json::array();
		for(const auto &metaentry : wavelenghts) {
			json entry;
			entry["filename"] = metaentry.filename;
			json targets = json::array();
			for(SpectreFloat val : metaentry.targets)
				targets.insert(targets.end(), val);

			entry["targets"] = targets;
			entry["norm_min_val"] = metaentry.norm_min_val;
			entry["norm_range"] = metaentry.norm_range;
			wavelenghts_array.insert(wavelenghts_array.end(), entry);
		}
		meta["wavelenghts"] = wavelenghts_array;


		stream << meta.dump(2);
		stream.flush();
	}

	void Metadata::load(std::istream &stream)
	{
		(void) stream;
	}

	void save_spectre(const std::string &path, const Spectre &spectre)
	{
		std::ofstream file(path, std::ios::trunc);
		if(!file) throw std::runtime_error("Cannot open file");

		for(const auto &e : spectre.get_map()) {
			file << std::format("{:.2f} {:.2f}\n", e.first, e.second);
		}

		file.flush();
	}

	void save_png_multichannel(std::ostream &stream, const SavingContext &ctx, const std::vector<SpectreFloat> &wavelenghts, SavingResult &res, int requested_channels)
	{
		const int vector_size = wavelenghts.size();
		if(vector_size < 1 || vector_size > 4) throw std::invalid_argument("Illegal wavelenghts size");

		int channels = vector_size > requested_channels ? vector_size : requested_channels;

		unsigned char *buf = new unsigned char[ctx.width * ctx.height * channels];
		if(buf == nullptr) throw std::bad_alloc();


		normalize_and_convert_to_rgb(ctx, buf, wavelenghts, channels, res.norm_range, res.norm_min);

		int code = write_png_to_stream(stream, ctx.width, ctx.height, channels, buf);
		delete[] buf;
		if(!code) {
			throw std::runtime_error("Error saving to file");
		}
		res.channels_used = channels;
	}

	void save_png_1(std::ostream &stream, const SavingContext &ctx, SpectreFloat wavelenght, SavingResult &res)
	{
		unsigned char *buf = new unsigned char[ctx.width * ctx.height];
		if(buf == nullptr) {
			res.success = false;
			throw std::bad_alloc();
		}

		normalize_and_convert_to_rgb(ctx, buf, {wavelenght}, 1, res.norm_range, res.norm_min);

		int code = write_png_to_stream(stream, ctx.width, ctx.height, 1, buf);
		delete[] buf;
		if(!code) {
			res.success = false;
			throw std::runtime_error("Error saving to file");
		}
		res.channels_used = 1;
	}

}