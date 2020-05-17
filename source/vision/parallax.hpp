#pragma once

#include <window/main_window.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d.hpp>


namespace robotica {
    inline cv::Mat parallax_depth_map(cv::Mat left, cv::Mat right) {
        auto& window = main_window::instance();

        cv::Mat left_grayscale, right_grayscale;
        cv::cvtColor(left,  left_grayscale,  cv::COLOR_BGR2GRAY);
        cv::cvtColor(right, right_grayscale, cv::COLOR_BGR2GRAY);

        auto stereo = cv::StereoBM::create(window.num_disparities * 16, window.block_size * 2 + 1);
        stereo->setDisp12MaxDiff(window.disparity_max_diff);
        stereo->setSpeckleRange(window.speckle_range);
        stereo->setSpeckleWindowSize(window.speckle_win_size * 2 + 1);
        stereo->setUniquenessRatio(window.uniqueness_ratio);
        stereo->setTextureThreshold(window.texture_threshold);
        stereo->setMinDisparity(window.min_disparity);
        stereo->setPreFilterCap(window.pre_filter_cap);
        stereo->setPreFilterSize(window.pre_filter_size);

        cv::Mat disparity;

        try {
            stereo->compute(left_grayscale, right_grayscale, disparity);
            cv::normalize(disparity, disparity, 0, 255, cv::NORM_MINMAX, CV_8U);
        } catch (...) {
            disparity = cv::Scalar(0);
        }

        return disparity;
    }
}