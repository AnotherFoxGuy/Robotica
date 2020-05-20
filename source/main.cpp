#include <window/main_window.hpp>
#include <world/robot.hpp>

int main(int argc, char** argv) {
    EASY_PROFILER_ENABLE;
    robotica::main_window& window = robotica::main_window::instance();

    while(robotica::robot::instance().update()) {
        window.update();
    }
}
