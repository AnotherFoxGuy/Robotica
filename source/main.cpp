#include <world/controller.hpp>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>

int main(int argc, char** argv) {
    ix::initNetSystem();

    //EASY_PROFILER_ENABLE;

    while (robotica::controller::instance().update());
}