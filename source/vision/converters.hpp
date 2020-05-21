#pragma once

#include <utility/typedefs.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <webots/Camera.hpp>
#include <glm/glm.hpp>

#include <filesystem>
#include <algorithm>

#include <easy/profiler.h>
#define PROFILE_PARALLEL 1


namespace robotica {
    const inline fs::path gradient_file_path = ROOT_DIR "/assets/gradient.png";


    inline cv::Mat inline_cvt(cv::Mat source, cv::ColorConversionCodes code) {
        cv::Mat result;
        cv::cvtColor(source, result, code);
        return result;
    }


    // Converts to color by showing the magnitude of the matrix as a gradient.
    inline cv::Mat grayscale_to_bgr(cv::Mat m) {
        const static cv::Mat gradient = cv::imread(gradient_file_path.string());
        auto get_color = [](double magnitude) { return (std::size_t) ((magnitude / 255.0) * gradient.cols); };

        cv::Mat result(m.cols, m.rows, CV_8UC3);

        std::size_t index = 0;
        for (std::size_t x = 0; x < m.cols; ++x) {
            for (std::size_t y = 0; y < m.rows; ++y) {
                const u8* color = &gradient.data[get_color(m.data[index])];

                for (std::size_t n = 0; n < 3; ++n) result.data[3 * index + n] = color[n];

                ++index;
            }
        }

        return result;
    }
}