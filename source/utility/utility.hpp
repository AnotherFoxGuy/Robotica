#pragma once

#include <opencv2/imgcodecs.hpp>

#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>


namespace robotica {
    inline std::string datetime_string(std::string_view fmt = "%Y%m%d_%H%M%S") {
        auto time = std::time(nullptr);
        auto localtime = std::localtime(&time);

        std::stringstream stream;
        stream << std::put_time(localtime, fmt.data());

        return stream.str();
    }


    inline void image_to_file(std::string_view folder, std::string_view filename_root, cv::Mat image) {
        if (!fs::exists(folder)) fs::create_directories(folder);

        fs::path dest_root = fs::path(folder) / filename_root;
        fs::path dest = dest_root;

        int i = 0;
        while (fs::exists(dest)) dest = dest_root.string() + "_" + std::to_string(i++);

        cv::imwrite(dest.string() + ".png", image);
    }
}