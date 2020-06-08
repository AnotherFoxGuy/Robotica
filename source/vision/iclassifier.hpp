#pragma once

#include <utility/typedefs.hpp>

#include <opencv2/opencv.hpp>

#include <vector>
#include <string>


namespace robotica {
    const inline fs::path classifier_folder = ROOT_DIR "/assets/";


    struct classified_object {
        cv::Rect bounding_rect;
        int confidence;
        std::string type;
    };


    class iclassifier {
    public:
        virtual ~iclassifier() = default;

        virtual std::vector<classified_object> classify(const cv::Mat& image) const = 0;

        const std::string& get_name(void) const {
            return name;
        }
    protected:
        iclassifier(std::string_view name) : name(name) {}
    private:
        std::string name;
    };
}