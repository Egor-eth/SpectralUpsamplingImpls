#include <utility>
#include <stdexcept>
#include <filesystem>
#include "spectral_image.h"
#include "spectral_util.h"

namespace fs = std::filesystem;

namespace 
{

	const std::string META_FILENAME = "meta.json";

	enum SaveFormat
	{
		PNG1,
		PNG1_JSON,
		PNG1_ZIP,
		PNG3,
		PNG3_JSON,
		PNG3_ZIP,
		ONE_SPECTRE,
		UNKNOWN
	};

	const std::unordered_map<std::string, SaveFormat> str_to_format{
		{"png1", PNG1},
		{"json", PNG1_JSON},
		{"zip", PNG1_ZIP},
		{"png1_zip", PNG1_ZIP},
		{"png3", PNG3},
		{"json3", PNG3_JSON},
		{"png3_zip", PNG3_ZIP},
		{"one_spectre", ONE_SPECTRE},
		{"spd", ONE_SPECTRE}
	};

	inline void filecheck(const std::string &path, bool &is_directory, bool &exists)
	{
		auto p = fs::path(path);
		exists = fs::exists(p);
		if(exists) {
			is_directory = fs::is_directory(p);
		}
	}

	inline SaveFormat get_type_from_path(const std::string &path)
	{
		fs::path p = path;
		std::string extension = p.extension();
		if(extension.empty()) return PNG1;
		else if(extension == "json") return PNG1_JSON;
		else if(extension == "zip") return PNG1_ZIP;
		else if(extension == "spd") return ONE_SPECTRE;
		return UNKNOWN;
 	}

 	inline SaveFormat get_type(const std::string &path, const std::string &format)
	{
		auto it = str_to_format.find(format);
		if(it != str_to_format.end()) {
			return it->second;
		} else {
			return get_type_from_path(path);
		}
 	}

 	bool save_as_png1(const fs::path &dir, const fs::path &meta, const SpectralImage &image) {
 		(void) dir;
 		(void) meta;
 		(void) image;
 		return false;
 	}

 	bool save_as_png3(const fs::path &dir, const fs::path &meta, const SpectralImage &image) {
		(void) dir;
 		(void) meta;
 		(void) image;
 		return false;
 	}

}

bool SpectralSaver::operator()(const std::string &path, const BaseImage<Spectre, void, SpectralSaver> &image_, const std::string &format) const
{
	const SpectralImage &image = static_cast<const SpectralImage &>(image_); // Bad practice :(
	if(image.get_width() == 1 && image.get_height() == 1) {
		spectral::save_spectre(path, image.at(0, 0));
	} else {
		bool is_directory;
		bool exists;
		filecheck(path, is_directory, exists);

		SaveFormat type = get_type(path, format);

		fs::path target_directory;
		fs::path target_file;
		if(is_directory) {
			target_directory = fs::path(path);
		} else {
			target_directory = fs::path(path).remove_filename();
			target_file = fs::path(path);
		}

		switch(type) {
		case PNG1:
			target_file = target_directory / META_FILENAME;
			[[fallthrough]];
		case PNG1_JSON:
			return save_as_png1(target_directory, target_file, image);
		case PNG3:
			target_file = target_directory / META_FILENAME;
			[[fallthrough]];
		case PNG3_JSON:
			return save_as_png3(target_directory, target_file, image);
		default:
			return false;
		}



	}
	return true;
}

template class BaseImage<Spectre, void, SpectralSaver>;