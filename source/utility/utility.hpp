#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>

#include "typedefs.hpp"

namespace robotica
{
inline std::string datetime_string(std::string_view fmt = "%Y%m%d_%H%M%S")
{
    auto time = std::time(nullptr);
    auto localtime = std::localtime(&time);

    std::stringstream stream;
    stream << std::put_time(localtime, fmt.data());

    return stream.str();
}

inline void image_to_file(std::string_view folder, std::string_view filename_root, cv::Mat image)
{
    if (!fs::exists(folder)) fs::create_directories(folder);

    fs::path dest_root = fs::path(folder) / filename_root;
    fs::path dest = dest_root;

    int i = 0;
    while (fs::exists(dest)) dest = dest_root.string() + "_" + std::to_string(i++);

    cv::imwrite(dest.string() + ".png", image);
}

inline cv::Mat crop_image(cv::Mat source, cv::Rect area)
{
    // TODO: Refactor this into something less wasteful. Default OpenCV crop method doesn't work.
    cv::Mat dest(area.width, area.height, source.type());

    int index_og = 0, index_new = 0;
    for (int x = 0; x < source.cols; ++x)
    {
        for (int y = 0; y < source.rows; ++y)
        {
            if (x >= area.x && x < area.x + area.width)
            {
                if (y >= area.y && y < area.y + area.height)
                {
                    for (int i = 0; i < source.channels(); ++i) dest.data[index_new + i] = source.data[index_og + i];
                    ++index_new;
                }
            }

            ++index_og;
        }
    }

    return dest;
}
} // namespace robotica