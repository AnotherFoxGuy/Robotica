#pragma once

#include <vision/iclassifier.hpp>
#include <utility/typedefs.hpp>
#include <window/main_window.hpp>

#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <string_view>
#include <filesystem>
#include <string>
#include <cassert>
#include <vector>


namespace robotica {
    class cascade_classifier : public iclassifier {
    public:
        cascade_classifier(std::string_view filename, std::string_view objname) :
            iclassifier(objname),
            filename(filename),
            classifier((classifier_folder / filename).string())
        {}


        std::vector<classified_object> classify(const cv::Mat& image) const override {
            auto& settings = main_window::instance();

            cv::Mat grayscale;
            cv::cvtColor(image, grayscale, cv::COLOR_BGR2GRAY);
            cv::equalizeHist(grayscale, grayscale);

            std::vector<cv::Rect> rects;
            std::vector<int> rejects;
            std::vector<double> weights;

            classifier.detectMultiScale(
                grayscale, 
                rects, 
                rejects, 
                weights, 
                settings.scale, 
                settings.min_neighbours, 
                0, 
                cv::Size(settings.min_obj_size, settings.min_obj_size), 
                cv::Size(settings.max_obj_size, settings.max_obj_size), 
                true
            );

            assert(rects.size() == confidence.size());

            std::vector<classified_object> results;
            for (int i = 0; i < rects.size(); ++i) results.push_back({ std::move(rects[i]), rejects[i], std::string(get_name()) });

            return results;
        }
    private:
        std::string filename;
        mutable cv::CascadeClassifier classifier;   // Not const correct.
    };
}