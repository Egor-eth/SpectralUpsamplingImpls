#include "argparse.h"
#include <cstring>
#include <getopt.h>
#include <iostream> 
#include <filesystem>

using namespace spec;

namespace fs = std::filesystem;

namespace {
    enum InputType
    {
        NONE,
        COLOR,
        FILE
    };
}

bool parse_args(int argc, char **argv, Args &args)
{
    static struct option long_options[] = {
        {"downsample", 0, nullptr, 1}
    };

    bool fill_name = false;
    InputType input_type = InputType::NONE;
    int c;
    fs::path p;
    while((c = getopt_long(argc, argv, "c:f:m:W;", long_options, nullptr)) != -1) {
        switch(c) {
        case 1:
            args.downsample_mode = true;
            break;
        case 'c':
            if(input_type != InputType::NONE) return false;
            args.color = Pixel::from_argb(std::stoi(optarg, nullptr, 16));
            input_type = InputType::COLOR;
            break;
        case 'f':
            if(input_type != InputType::NONE) return false;
            args.input_path = optarg;
            input_type = InputType::FILE;
            break;
        case 'm':
            args.method.emplace(optarg);
            break;
        case 'o':
            p = optarg;
            args.output_dir.emplace(p.relative_path());
            args.output_name.emplace(p.stem());
            break;
        case 'D':
            args.output_dir.emplace(optarg);
            fill_name = true;
            break;
        case 'W':
            if(!strcmp(optarg, "downsample")) {
                args.downsample_mode = true;
            }
            break;
        case '?':
            std::cerr << "[!] Unknown argument." << std::endl; 
            return false;
        }
    }  
    if(input_type == InputType::NONE) {
        std::cerr << "[!] No input specified." << std::endl;
        return false;
    }

    if(fill_name) {
        if(args.output_name) {
            std::cerr << "[!] Options -o and -D cannot be used simultaneously." << std::endl;
            return false;
        }
        if(input_type != InputType::FILE) {
            std::cerr << "[!] Option -D needs input file to be specified with -f." << std::endl;
            return false;
        }
        fs::path p1{args.input_path};
        args.output_name.emplace(p.stem());
    }

    //validate
    if(args.downsample_mode) {
        return input_type == InputType::FILE && !args.output_dir.has_value() && !args.output_name.has_value() && !args.method.has_value();
    }
    else {
        if(!args.method) {
            std::cerr << "[!] No method specified." << std::endl;
            return false;
        }

        return args.output_dir.has_value();
    }
}