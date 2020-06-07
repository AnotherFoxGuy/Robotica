#pragma once

#include <vision/iclassifier.hpp>
#include <utility/typedefs.hpp>
#include <utility/utility.hpp>
#include <utility/math.hpp>
#include <window/main_window.hpp>

#include <opencv2/imgproc.hpp>


namespace robotica { 
    class pool_classifier : public iclassifier {
    public:
        pool_classifier(void) : iclassifier("Pool") {}


        std::vector<classified_object> classify(const cv::Mat& image) const override {
            auto& settings = main_window::instance();

            cv::Mat grayscale;
            cv::cvtColor(image, grayscale, cv::COLOR_BGR2GRAY);
            cv::bitwise_not(grayscale, grayscale);
            cv::threshold(grayscale, grayscale, settings.pool_grayscale_cutoff, 255, cv::THRESH_BINARY);

            static const char name[] = "grayscale";
            show<name>(grayscale);

            std::vector<std::vector<cv::Point>> contours;
            std::vector<cv::Vec4i> hierarchy;

            cv::Mat canny;
            cv::Canny(grayscale, canny, 100, 200);
            cv::findContours(canny, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

            std::vector<classified_object> result;

            for (const auto& contour : contours) {
                double perimeter   = cv::arcLength(contour, true);
                double area        = cv::contourArea(contour);
                double circularity = (4 * pi * area) / (perimeter * perimeter);
                cv::Rect bounding  = cv::boundingRect(contour);

                cv::Vec3b bgr = image.at<cv::Vec3b>(cv::Point{ bounding.x + (bounding.width / 2), bounding.y + (bounding.height / 2) });

                auto diff = [](const auto& a, const auto& b) { return (double) std::abs(a - b); };
                double colourfullness = (diff(bgr[0], bgr[1]) + diff(bgr[0], bgr[2]) + diff(bgr[1], bgr[2])) / (3 * 255);


                if (area           < settings.pool_min_area           || area           > settings.pool_max_area          ) continue;
                if (circularity    < settings.pool_min_circularity    || circularity    > settings.pool_max_circularity   ) continue;
                if (perimeter      < settings.pool_min_perimeter      || perimeter      > settings.pool_max_perimeter     ) continue;
                if (colourfullness < settings.pool_min_colourfullness || colourfullness > settings.pool_max_colourfullness) continue;


                result.push_back({
                    bounding,
                    1000,
                    get_name()
                });
            }

            return result;
        }
    };
}