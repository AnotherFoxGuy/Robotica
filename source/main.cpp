#include "SoundAnalyzer.hpp"
#include <world/controller.hpp>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>

int main(int argc, char** argv) {
    ix::initNetSystem();

    //EASY_PROFILER_ENABLE;
    SoundAnalyzer x;
    x.scan_mp3();

    while (robotica::controller::instance().update());
}
