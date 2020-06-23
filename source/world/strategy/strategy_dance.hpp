#pragma once
#include "dance/MusicAnalyzer.hpp"
#include "dance/strategies/dance_moves.hpp"
#include "dance/strategies/idance_strategy.hpp"
#include "world/controller.hpp"
#include "world/strategy/istrategy.hpp"
#include "comms/websocket.hpp"

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
        //printf("loading file \n");
        //music_analyzer_.load_file(song_path);
        //music_analyzer_.PrintSum();
        orignal_frame_count = controller_.frame;
    }

    void exit(void) override { frame = 0; }
    
    void loop(void) override
    {
        int total = 0;
        auto x =websocket::instance().audio_buffer;
        for (int i =0; i < 15 ; i++)
        {
           total += x[i];
        }
        average1= total/16;
        total =0;
        for (int j = 15; j < 31; j ++)
        {
            total += x[j];
        }
        average2 = total / 16;
        for (int k = 31 ; k < 47; k++)
        {
            total += x[k];
        }
        average3 = total / 16;
        total = 0;
        for (int l = 47; l < 63; l++)
        {
            total += x[l];
        }
        average4 = total / 16;
        total = 0;
        std::cout << average1 << " " << average2 << " " << average3 << " " << average4 << std::endl;
        if ((controller_.frame - orignal_frame_count) % 5 == 0)
        {
            //data[x][y];
            frame = controller_.frame - orignal_frame_count;
            if(average1 > 50 )
            {
                dance_moves_.move_forward(frame, 1);
            }
            else if ( average1 < 50 && average1 != 0)
            {
                dance_moves_.move_forward(frame, -1);
            }
            else
            {
                dance_moves_.move_forward(frame, 0);
            }

            if (average2 > 50 && average2 < 100)
            {
                dance_moves_.move_long(frame, 1);
            }
            else if ( average2 > 100)
            {
                dance_moves_.rotate_arm(frame);
            }
            else if (average2 < 50 && average2 != 0)
            {
                dance_moves_.move_long(frame, -1);
            }else
            {
                dance_moves_.move_long(frame, 0);
            }

             if (average3 > 50)
            {
                dance_moves_.roll_gripper(frame,1);
            }
            else if (average3 < 50 && average3 != 0)
            {
                dance_moves_.roll_gripper(frame, -1);
            }
            else
            {
                dance_moves_.roll_gripper(frame, 0);
            }

            if (average4 > 50)
            {
                dance_moves_.rotate_body(frame, 1);
            }
            else if (average4 < 50 && average3 != 0)
            {
                dance_moves_.rotate_body(frame, -1);
            }
            else
            {
                dance_moves_.rotate_body(frame, 0);
            }







            if (frame == 30)
            {
                orignal_frame_count = controller_.frame;
                frame = 0;
                amount_of_seconds_danced++;
                
            }
        }
    };
    private:
    float average1;
    float average2;
    float average3;
    float average4;
    int amount_of_seconds_danced = 0;
    int orignal_frame_count;
    int frame = 0;
    std::string song_path;
    idance_strategy strategy;
    dance_moves& dance_moves_ = dance_moves::instance();
    controller& controller_ = controller::instance();
    MusicAnalyzer& music_analyzer_ = MusicAnalyzer::instance();
};
}

