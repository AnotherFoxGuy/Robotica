#pragma once

#include <utility/typedefs.hpp>

#include <glm/glm.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <webots/Camera.hpp>

#include <algorithm>
#include <filesystem>

namespace robotica
{
const inline fs::path gradient_file_path = ROOT_DIR "/assets/gradient.png";

inline cv::Mat inline_cvt(cv::Mat source, cv::ColorConversionCodes code)
{
    cv::Mat result;
    cv::cvtColor(source, result, code);
    return result;
}

// Converts to color by showing the magnitude of the matrix as a gradient.
inline cv::Mat grayscale_to_bgr(cv::Mat m)
{
    const static cv::Mat gradient = cv::imread(gradient_file_path.string());
    auto get_color = [](double magnitude) { return (std::size_t)((magnitude / 255.0) * gradient.cols); };

    cv::Mat result(m.cols, m.rows, CV_8UC3);

    std::size_t index = 0;
    for (std::size_t x = 0; x < m.cols; ++x)
    {
        for (std::size_t y = 0; y < m.rows; ++y)
        {
            const u8* color = &gradient.data[get_color(m.data[index])];

            for (std::size_t n = 0; n < 3; ++n) result.data[3 * index + n] = color[n];

            ++index;
        }
    }

    return result;
}

inline cv::Mat camera_format_to_bgr(const unique<webots::Camera>& source, glm::ivec2 viewport)
{
    cv::Mat result(viewport.y, viewport.x, CV_8UC3);

    // The documentation for Webots about the internal format of the camera data makes the following claims:
    // - The image is stored in RGB
    // - The image is stored in BGRA
    // - The image data is in array-of-structs format and not struct-of-arrays format.
    // Some of these claims contradict each other and none of them appear to be correct.
    // Fortunately, we can bypass this mess by just iterating over each pixel and using the built in functions
    // to extract image data for each channel.
    unsigned index = 0;
    for (int x = 0; x < viewport.x; ++x)
    {
        for (int y = 0; y < viewport.y; ++y)
        {
            result.data[index + 0] = source->imageGetBlue(source->getImage(), viewport.x, x, y);
            result.data[index + 1] = source->imageGetGreen(source->getImage(), viewport.x, x, y);
            result.data[index + 2] = source->imageGetRed(source->getImage(), viewport.x, x, y);

            index += 3;
        }
    }

    cv::rotate(result, result, cv::ROTATE_90_CLOCKWISE);
    cv::flip(result, result, 1);

    return result;
}
} // namespace robotica