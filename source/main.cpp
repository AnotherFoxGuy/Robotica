#include <world/controller.hpp>


int main(int argc, char** argv) {
    EASY_PROFILER_ENABLE;

    while (robotica::controller::instance().update());
}
