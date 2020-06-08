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

            cv::Mat shown;
            cv::cvtColor(color_detect_map, shown, cv::COLOR_GRAY2BGR);

            int index = 0;
            for (const auto& contour : contours) {
                // Only take top-level contours.
                if (hierarchy[index++][3] != -1) continue;

                cv::Rect bounding     = cv::boundingRect(contour);
                cv::Point center      = bounding.tl() + cv::Point{ bounding.size() / 2 };
                double perimeter      = cv::arcLength(contour, true);
                double area           = floodfill_area(color_detect_map, center);
                double circularity    = (4 * pi * area) / (perimeter * perimeter);
                double colourfullness = ((double) colorfullness.at<uchar>(cv::Point{ bounding.x + (bounding.width / 2), bounding.y + (bounding.height / 2) })) / 255;


                auto getclr = [&](const cv::Point& pt) { return image.at<cv::Vec3b>(pt); };

                unsigned long long variance = settings.pool_variance_base, pixels = 0;
                cv::Vec3b reference = getclr(center);
                floodfill_foreach(color_detect_map, center, [&](const auto& pt) {
                    ++pixels;

                    cv::Vec3b value = getclr(pt);
                    for (int i = 0; i < reference.channels; ++i) {
                        variance += std::abs(((int) reference[i]) - value[i]);
                    }
                });


                cv::putText(shown, std::to_string(variance / pixels), bounding.tl(), cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 0, 255));
                cv::putText(shown, std::to_string(area), bounding.tl() + cv::Point { 0, 15 }, cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 255, 0));


                if (variance / pixels > settings.pool_max_variance      ) continue;
                if (area              < settings.pool_min_area          ) continue;
                if (area              > settings.pool_max_area          ) continue;
                if (perimeter         < settings.pool_min_perimeter     ) continue;
                if (colourfullness    < settings.pool_min_colourfullness) continue;
                if (bounding.width    < settings.pool_min_width         ) continue;


                // If the object is small, relax the roundness requirements, since it is too heavily influenced by the resolution of the camera.
                if (area > settings.pool_area_mode_switch && circularity < settings.pool_min_circularity) continue;
                // Only enforce oblongness at a distance, since objects get more round the closer they are.
                if (area < settings.pool_area_mode_switch && bounding.width / bounding.height < settings.pool_min_oblongness) continue;


                result.push_back({
                    bounding,
                    1000,
                    get_name()
                });
            }

            static const char name[] = "tmp";
            show<name>(shown);

            return result;
        }
    private:
        static cv::Mat colourfullness(const cv::Mat& src) {
            auto& settings = main_window::instance();
            cv::Mat dst(src.rows, src.cols, CV_8UC1);

            std::transform(
                std::execution::par_unseq,
                src.begin<cv::Vec3b>(),
                src.end<cv::Vec3b>(),
                dst.begin<uchar>(),
                [&](const cv::Vec3b& bgr) {
                    int avg = (((int) bgr[0]) + bgr[1] + bgr[2]) / 3;

                    auto diff = [](const auto& a, const auto& b) { return (int) std::abs(a - b); };
                    int colourfullness = (diff(bgr[0], bgr[1]) + diff(bgr[0], bgr[2]) + diff(bgr[1], bgr[2])) / 2;
                    colourfullness += std::powf(avg * settings.pool_white_boost, settings.pool_white_scale) / 255;
                    colourfullness = std::clamp(colourfullness, 0, 255);

                    return (uchar) colourfullness;
                }
            );

            return dst;
        }
    };
}