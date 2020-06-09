#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <utility/utility.hpp>


namespace robotica {
    const inline fs::path temperature_file_path = ROOT_DIR "/assets/temperature.png";


    enum class temperature { COLD, TEPID, WARM, HOT };


    inline float calculate_temperature(const cv::Vec3b& color) {
        const static cv::Mat gradient = cv::imread(temperature_file_path.string());

        auto diff = [](const cv::Vec3b& a, const cv::Vec3b& b) {
            cv::Vec3b result;
            for (int i = 0; i < result.channels; ++i) result[i] = std::abs(b[i] - a[i]);
            return (result[0] + result[1] + result[2]) / 3.0;
        };

        int best = 0;
        double best_diff = diff(gradient.at<cv::Vec3b>(0, 0), color);
        for (int i = 1; i < gradient.cols; ++i) {
            auto d = diff(gradient.at<cv::Vec3b>(0, i), color);

            if (d < best_diff) {
                best = i;
                best_diff = d;
            }
        }

        return ((float) best) / gradient.cols;
    }


    inline temperature temperature_class(float v) {
        if (v < 0.30) return temperature::COLD;
        if (v < 0.53) return temperature::TEPID;
        if (v < 0.76) return temperature::WARM;
        return temperature::HOT;
    }
}
