#include <world/controller.hpp>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include "dance/MusicAnalyzer.hpp"

int main(int argc, char** argv) {
    ix::initNetSystem();

    //EASY_PROFILER_ENABLE;
    
    if(MusicAnalyzer::instance().loadfile())
    {
        MusicAnalyzer::instance().SetData();
        MusicAnalyzer::instance().SetConfig();
    }
    

    while (robotica::controller::instance().update());
}
