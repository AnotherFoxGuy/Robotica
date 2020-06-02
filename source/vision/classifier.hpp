#pragma once

#include <utility/typedefs.hpp>

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
    const inline fs::path classifier_folder = ROOT_DIR "/assets/";


    struct detected_object {
        cv::Rect bounding_rect;
        double confidence;
    };


    inline std::vector<detected_object> classify(cv::Mat source, cv::CascadeClassifier& classifier) {
        cv::Mat grayscale;
        cv::cvtColor(source, grayscale, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(grayscale, grayscale);

        std::vector<cv::Rect> rects;
        std::vector<int> rejects;
        std::vector<double> confidence;

        classifier.detectMultiScale(grayscale, rects, rejects, confidence, 1.1, 3, 0, cv::Size(), cv::Size(), true);
        assert(rects.size() == confidence.size());

        std::vector<detected_object> results;
        for (int i = 0; i < rects.size(); ++i) results.push_back({ std::move(rects[i]), confidence[i] });

        return results;
    }
}