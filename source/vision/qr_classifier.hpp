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
namespace robotica
{
    class qr_classifier : public robotica::iclassifier
    {
      public:
        qr_classifier() : iclassifier("qr"),
                                                                                  filename(filename),
                                                                                  classifier((classifier_folder / filename).string())
        {
        }

        std::vector<classified_object> classify(const cv::Mat& image) const override
        {
            auto& settings = main_window::instance();

            cv::QRCodeDetector qr_detector = cv::QRCodeDetector::QRCodeDetector();
            cv::Mat bbox, rectifiedImage;
            std::string data = qr_detector.detectAndDecode(image, bbox, rectifiedImage);
            if (data.length() > 0)
            {
                std::cout << "decoded data:" << data << std::endl;
                rectifiedImage.convertTo(rectifiedImage, CV_8UC3);
                imshow("Rectified QRCode", rectifiedImage);
            }
            else
                std::cout << "Qr not found" << std::endl;
            return {};
        }
        

      private:
        std::string filename;
        mutable cv::CascadeClassifier classifier; // Not const correct.
    };
}