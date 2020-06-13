#include <world/controller.hpp>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocket.h>
#include "dance/MusicAnalyzer.hpp"

int main(int argc, char** argv) {
    ix::initNetSystem();

    //EASY_PROFILER_ENABLE;
    MusicAnalyzer music;
    if(music.loadfile())
    {
        music.printInfo();
       music.getSampels();
    }
    

    while (robotica::controller::instance().update());
}
