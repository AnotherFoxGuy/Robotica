#pragma once
#include "dance/MusicAnalyzer.hpp"
#include "dance/strategies/dance_moves.hpp"
#include "dance/strategies/idance_strategy.hpp"
#include "world/controller.hpp"
#include "world/strategy/istrategy.hpp"

#include <string>


namespace robotica
{
int contr;

class strategy_dance : public istrategy
{
  public:
    strategy_dance(std::string song)
    {
        song_path = ROOT_DIR "/music/" + song;
        printf("strategy selected\n");
    }
    void init(void) override
    {
        printf("loading file \n");
        music_analyzer_.load_file(song_path);
        music_analyzer_.PrintSum();
        orignal_frame_count = controller_.frame;
    }

    void exit(void) override { frame = 0; }

    void loop(void) override
    {
        if ((controller_.frame - orignal_frame_count) % 5 == 0)
        {
            frame = controller_.frame - orignal_frame_count; 
            set_strat(arm_dance_move_3::instance());
            set_strat(track_dance_move_3::instance());
            if (frame == 30)
            {
                orignal_frame_count = controller_.frame;
                frame = 0;
                amount_of_seconds_danced++;
            }
        }
    };
    void set_strat(idance_strategy &strat)
    {
        if (amount_of_seconds_danced != 4)
            strat.dance(frame);
    }
    private:
    int amount_of_seconds_danced = 0;
    int orignal_frame_count;
    int frame = 0;
    std::string song_path;
    idance_strategy strategy;
    controller& controller_ = controller::instance();
    MusicAnalyzer& music_analyzer_ = MusicAnalyzer::instance();
};
}

