#ifndef ARGPARSE_H
#define ARGPARSE_H
#include <string>
#include <optional>
#include "imageutil/pixel.h"


struct Args {
    std::optional<std::string> output_name; //-n
    std::optional<std::string> method; // -m
    std::optional<spec::Pixel> color; // -c

    std::string output_dir = "output";
    std::string input_path; // -f
    bool downsample_mode = false; // --downsample
};

bool parse_args(int argc, char **argv, Args &args);


#endif