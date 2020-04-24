#include <world/robot.hpp>

#include <opencv2/opencv.hpp>

#include <string>


constexpr int timestep = 64;

int main(int argc, char** argv) {
    robotica::robot robot { timestep };

    for (auto side : robotica::sides) {
        const auto viewport = robot.get_camera_viewport_size(side);
        const std::string name = (side == robotica::side::LEFT) ? "Left Camera Output" : "Right Camera Output";

        cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
        cv::resizeWindow(name, { viewport.y, viewport.x });
    }

    while (robot.update()) {
        for (auto side : robotica::sides) {
            const auto camera_output = robot.get_camera_output(side);
            const std::string name = (side == robotica::side::LEFT) ? "Left Camera Output" : "Right Camera Output";

            cv::imshow(name, camera_output);
            cv::waitKey(1);
        }
    }

    return 0;
}
