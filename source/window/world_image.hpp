#pragma once

#include <vision/world_model.hpp>
#include <world/robot.hpp>

#include <string_view>


namespace robotica {
    inline cv::Mat generate_world_map(void) {
        constexpr int image_size = 255;

        std::vector<world_model::world_object> objects = world_model::instance().get_object_list();
        objects.push_back({ glm::vec2{ 0, 0 }, 1.0, 1.0, "RBT" });

        cv::Mat result(255, 255, CV_8UC3);
        result = cv::Scalar(255, 255, 255);

        for (const auto& detection : objects) {
            std::string text = "[" + detection.identity + "]";

            int baseline;
            auto text_size = cv::getTextSize(text, cv::FONT_HERSHEY_PLAIN, 1.0, 1, &baseline);

            cv::Point imgpos = cv::Point2f{ image_size / 2, image_size / 2 } - cv::Point2f{ detection.position.x, detection.position.y };
            cv::Point txtpos = imgpos - cv::Point{ text_size / 2 };

            cv::putText(result, text, txtpos, cv::FONT_HERSHEY_PLAIN, 1.0, cv::Scalar(0, 0, 0));
        }

        return result;
    }
}