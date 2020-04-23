#include <robot.hpp>

#include <opencv2/opencv.hpp>

#include <string>


constexpr int timestep = 64;
const std::string camera_window_name = "Robot Camera Output";

int main(int argc, char** argv) {
    robotica::robot robot { timestep };
    const auto viewport = robot.get_camera_viewport_size();

    cv::namedWindow(camera_window_name, cv::WINDOW_AUTOSIZE);
    cv::resizeWindow(camera_window_name, { viewport.y, viewport.x });

    while (robot.update()) {
        const auto camera_output = robot.get_camera_output();

        cv::imshow(camera_window_name, camera_output);
        cv::waitKey(1);
    }

    return 0;
}
