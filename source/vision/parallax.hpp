#pragma once

#include <window/main_window.hpp>

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/ximgproc/disparity_filter.hpp>


namespace robotica {
    struct disparity_info {
        cv::Mat left_disp, right_disp, filtered, raw_vis, filtered_vis;
        cv::Rect roi;
    };


    inline disparity_info parallax_depth_map(cv::Mat left, cv::Mat right) {
        auto& window = main_window::instance();

        cv::Mat left_grayscale, right_grayscale;
        cv::cvtColor(left,  left_grayscale,  cv::COLOR_BGR2GRAY);
        cv::cvtColor(right, right_grayscale, cv::COLOR_BGR2GRAY);

        try {
            auto left_matcher  = cv::StereoBM::create(window.num_disparities * 16, window.block_size * 2 + 1);
            left_matcher->setDisp12MaxDiff(window.disparity_max_diff);
            left_matcher->setSpeckleRange(window.speckle_range);
            left_matcher->setSpeckleWindowSize(window.speckle_win_size * 2 + 1);
            left_matcher->setUniquenessRatio(window.uniqueness_ratio);
            left_matcher->setTextureThreshold(window.texture_threshold);
            //left_matcher->setMinDisparity(window.min_disparity);
            left_matcher->setPreFilterCap(window.pre_filter_cap);
            left_matcher->setPreFilterSize(window.pre_filter_size);

            auto right_matcher = cv::ximgproc::createRightMatcher(left_matcher);
            auto wls_filter = cv::ximgproc::createDisparityWLSFilter(left_matcher);


            cv::Mat left_disparity, right_disparity;
            left_matcher->compute(left_grayscale, right_grayscale, left_disparity);
            right_matcher->compute(right_grayscale, left_grayscale, right_disparity);

            wls_filter->setLambda(window.lambda);
            wls_filter->setSigmaColor(window.sigma);

            cv::Mat filtered;
            wls_filter->filter(left_disparity, left, filtered, right_disparity);


            auto confidence = wls_filter->getConfidenceMap();
            auto roi = wls_filter->getROI();

            cv::Mat raw_vis, filtered_vis;
            cv::ximgproc::getDisparityVis(left_disparity, raw_vis, window.raw_vis_scale);
            cv::ximgproc::getDisparityVis(filtered, filtered_vis, window.filtered_vis_scale);


            // Convert from CV_16S to CV_8U
            for (auto* mat : std::array { &left_disparity, &right_disparity, &filtered, &raw_vis, &filtered_vis }) {
                mat->convertTo(*mat, CV_8U);
            }

            return disparity_info {
                left_disparity, right_disparity, filtered, raw_vis, filtered_vis, roi
            };
        } catch (...) {
            cv::Mat dummy(left.rows, left.cols, CV_8U);
            dummy = cv::Scalar(0);

            return disparity_info { dummy, dummy, dummy, dummy, dummy, cv::Rect{} };
        }
    }
}