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
    class rock_classifier : public iclassifier {
    public:
        rock_classifier(void) : iclassifier("Rock") {}


        std::vector<classified_object> classify(const cv::Mat& image) const override {
            auto& settings = main_window::instance();

            auto gray = grayness(image);
            cv::threshold(gray, gray, settings.rock_grayness_threshold, 255, cv::THRESH_BINARY);

            std::vector<classified_object> result;

            std::vector<std::vector<cv::Point>> contours;
            std::vector<cv::Vec4i> hierarchy;

            cv::findContours(gray, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

            static const char n[] = "";
            cv::Mat m;
            cv::cvtColor(gray, m, cv::COLOR_GRAY2BGR);
            show<n>(m);

            int index = 0;
            for (const auto& contour : contours) {
                if (hierarchy[index++][3] != -1) continue; // Only take top-level contours.

                cv::Rect bounding     = cv::boundingRect(contour);
                cv::Point center      = bounding.tl() + cv::Point{ bounding.size() / 2 };
                double perimeter      = cv::arcLength(contour, true);
                double area           = cv::contourArea(contour);
                double circularity    = (4 * pi * area) / (perimeter * perimeter);

                // Don't detect shit in the sky
                if (bounding.tl().y < image.rows * 0.33) continue;

                if (area < settings.rock_min_size) continue;
                if (circularity < settings.rock_min_roundness) continue;

                result.push_back({
                    bounding,
                    1000,
                    get_name()
                });
            }


            return result;
        }
    private:
        static cv::Mat grayness(const cv::Mat& image) {
            cv::Mat result(image.rows, image.cols, CV_8UC1);

            auto colorfullness = [](const auto& v) {
                auto diff = [](uchar a, uchar b) { return std::abs(((int) b) - ((int) a)); };
                return std::clamp(diff(v[0], v[1]) + diff(v[0], v[2]) + diff(v[1], v[2]), 0, 255);
            };

            for (int x = 0; x < result.rows; ++x) {
                for (int y = 0; y < result.cols; ++y) {
                    const cv::Vec3b& clr = image.at<cv::Vec3b>(x, y);
                    result.at<uchar>(x, y) = 255 - colorfullness(clr);
                }
            }

            return result;
        }
    };
}