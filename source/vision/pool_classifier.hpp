#pragma once

#include <vision/iclassifier.hpp>
#include <vision/floodfill.hpp>
#include <utility/typedefs.hpp>
#include <utility/utility.hpp>
#include <utility/math.hpp>
#include <window/main_window.hpp>

#include <opencv2/imgproc.hpp>

#include <algorithm>
#include <execution>


namespace robotica { 
    class pool_classifier : public iclassifier {
    public:
        pool_classifier(void) : iclassifier("Pool") {}


        std::vector<classified_object> classify(const cv::Mat& image) const override {
            auto& settings = main_window::instance();

            cv::Mat color_detections = pool_colors(image);
            cv::threshold(color_detections, color_detections, settings.pool_color_threshold, 255, cv::THRESH_BINARY_INV);


            std::vector<classified_object> result;

            std::vector<std::vector<cv::Point>> contours;
            std::vector<cv::Vec4i> hierarchy;

            cv::findContours(color_detections, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);


            int index = 0;
            for (const auto& contour : contours) {
                if (hierarchy[index++][3] != -1) continue; // Only take top-level contours.

                cv::Rect bounding     = cv::boundingRect(contour);
                cv::Point center      = bounding.tl() + cv::Point{ bounding.size() / 2 };
                double perimeter      = cv::arcLength(contour, true);
                double area           = cv::contourArea(contour);
                double circularity    = (4 * pi * area) / (perimeter * perimeter);

                // Don't detect shit in the sky
                if (bounding.tl().y < image.rows / 2) continue;


                if (bounding.size().width < settings.pool_min_width) continue;
                if (circularity < settings.pool_min_roundness) continue;

                result.push_back({
                    bounding,
                    1000,
                    get_name(),
                    floodfill_avgcolor(color_detections, image, center)
                });
            }


            return result;
        }
    private:
        static cv::Mat pool_colors(const cv::Mat& image) {
            cv::Mat hsv;
            cv::cvtColor(image, hsv, cv::COLOR_BGR2HSV);

            auto& settings = main_window::instance();

            const static cv::Vec3b pool_colors[3] = {
                { 196, 28, 87 },
                { 10, 56, 86 },
                { 63, 54, 83 }
            };


            cv::Mat result(image.rows, image.cols, CV_8UC1);

            for (int x = 0; x < result.rows; ++x) {
                for (int y = 0; y < result.cols; ++y) {
                    const cv::Vec3b& clr = hsv.at<cv::Vec3b>(x, y);

                    int min_difference = std::numeric_limits<int>::max();
                    for (int i = 0; i < 3; ++i) {
                        auto diff = difference(clr, pool_colors[i]);
                        if (diff < min_difference) min_difference = diff;
                    }

                    result.at<uchar>(x, y) = 255 - std::clamp(min_difference, 0, 255);
                }
            }

            return result;
        }
    };
}