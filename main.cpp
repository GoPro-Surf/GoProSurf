#include <iostream>
#include <filesystem>
#include <exception>
#include "cxxopts.hpp"
#include "GpsInfo.h"
#include "Detector.h"
#include "Video.h"

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
    cxxopts::Options options("GoProSurfSplitter",
                             "It finds caught waves in GoPro videos with enabled GPS and writes them to new shorter files");

    options.add_options()
            ("d,directory", "Directory with *.MP4 files", cxxopts::value<std::string>()->default_value("./"))
            ("o,out", "Output directory", cxxopts::value<std::string>()->default_value("./waves"))
            ("m,margin", "Additional time before and after detected wave (sec)",
             cxxopts::value<size_t>()->default_value("3"))
            ("l,length", "Minimum wave length (sec)", cxxopts::value<size_t>()->default_value("2"))
            ("s,speed", "Minimum wave speed (m/s)", cxxopts::value<float>()->default_value("2.0"))
            ("h,help", "", cxxopts::value<bool>());

    cxxopts::ParseResult opts;
    try {
        opts = options.parse(argc, argv);

        if (opts.count("help")) {
            std::cout << options.help();
            return 0;
        }
    } catch (const cxxopts::OptionException &e) {
        std::cerr << "Error parsing command-line options: " << e.what() << std::endl;
        std::cerr << options.help();
        return -1;
    }

    if (!fs::exists(opts["out"].as<std::string>())) {
        if (!fs::create_directory(opts["out"].as<std::string>()))
            throw std::invalid_argument("Cannot create directory " + opts["out"].as<std::string>());
    }

    for (const auto &entry: fs::directory_iterator(opts["directory"].as<std::string>())) {
        if (entry.is_regular_file() && entry.path().extension() == ".MP4") {
            auto filename = entry.path().string();

            GpsInfo gps;
            gps.ReadFromVideo(filename);

            std::cout << "Processing file " << filename << std::endl;
            auto frames = Detector::GetWaves(gps, opts["margin"].as<size_t>(), opts["length"].as<size_t>(),
                                             opts["speed"].as<float>());
            if (frames.empty())
                std::cout << "\t No waves found" << std::endl;

            auto outFilenamePrefix = (fs::path(opts["out"].as<std::string>()) /
                                      fs::path(filename).filename().replace_extension("")).string();

            for (auto i = 0; i < frames.size(); ++i) {
                std::cout << "\tFound a wave between " << frames[i].start << " and " << frames[i].finish << " seconds"
                          << std::endl;

                auto outFilename = outFilenamePrefix + "-" + std::to_string(i) + ".MP4";
                std::cout << "\t\tWriting to " << outFilename << " ... ";
                Video::CopyFromTo(filename, outFilename, frames[i].start, frames[i].finish);
                std::cout << "done" << std::endl;
            }
        }
    }

    return 0;
}
