#pragma once
#include "dance/MusicAnalyzer.hpp"
#include "world/strategy/istrategy.hpp"

#include <string>


namespace robotica
{
class strategy_dance : public robotica::istrategy
{
  public:
    strategy_dance(std::string song)
    {
        song_path = ROOT_DIR "/music/" + song;
        printf("strategy selected");
    }
    void init(void) override
    {
        printf("loading file");
        music_analyzer_.load_file(song_path);
    }

    void exit(void) override{}

    void loop(void) override
    {
        
    };
    private:
    std::string song_path;
    MusicAnalyzer& music_analyzer_ = MusicAnalyzer::instance();
};
}

