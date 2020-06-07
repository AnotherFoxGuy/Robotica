#pragma once

#include <vision/iclassifier.hpp>
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

            cv::Mat colorfullness = colourfullness(image);

            cv::Mat color_detect_map;
            cv::threshold(colorfullness, color_detect_map, settings.pool_grayscale_cutoff, 255, cv::THRESH_BINARY);

            std::vector<std::vector<cv::Point>> contours;
            std::vector<cv::Vec4i> hierarchy;

            cv::findContours(color_detect_map, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

            std::vector<classified_object> result;

            int index = 0;
            for (const auto& contour : contours) {
                // Only take top-level contours.
                if (hierarchy[index++][3] != -1) continue;

                cv::Rect bounding     = cv::boundingRect(contour);
                double perimeter      = cv::arcLength(contour, true);
                double area           = floodfill_area_estimate(color_detect_map, bounding.tl() + cv::Point { bounding.size() / 2 });
                double circularity    = (4 * pi * area) / (perimeter * perimeter);
                double colourfullness = ((double) colorfullness.at<uchar>(cv::Point{ bounding.x + (bounding.width / 2), bounding.y + (bounding.height / 2) })) / 255;


                if (area           < settings.pool_min_area          ) continue;
                if (perimeter      < settings.pool_min_perimeter     ) continue;
                if (colourfullness < settings.pool_min_colourfullness) continue;


                // If the object is small, relax the roundness requirements, since it is too heavily influenced by the resolution of the camera.
                if (area > settings.pool_area_mode_switch && circularity < settings.pool_min_circularity) continue;


                result.push_back({
                    bounding,
                    1000,
                    get_name()
                });
            }

            return result;
        }
    private:
        static cv::Mat colourfullness(const cv::Mat& src) {
            cv::Mat dst(src.rows, src.cols, CV_8UC1);

            std::transform(
                std::execution::par_unseq,
                src.begin<cv::Vec3b>(),
                src.end<cv::Vec3b>(),
                dst.begin<uchar>(),
                [](const cv::Vec3b& bgr) {
                    auto diff = [](const auto& a, const auto& b) { return (int) std::abs(a - b); };
                    int colourfullness = (diff(bgr[0], bgr[1]) + diff(bgr[0], bgr[2]) + diff(bgr[1], bgr[2])) / 2;

                    return (uchar) colourfullness;
                }
            );

            return dst;
        }
    };
}