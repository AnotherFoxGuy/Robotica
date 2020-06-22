#pragma once
#include "dance/AudioFile.h"
#include "window/imgui_plot/imgui_plot.hpp"
#include "comms/websocket.hpp"

class MusicAnalyzer
{
    AudioFile<double> audioFile;
    std::vector<float> data;
    ImGui::PlotConfig conf;
  public:
    static MusicAnalyzer& instance(void);
    bool load_file(std::string song_path)
    {
         if(audioFile.load(song_path))
         {
             SetData();
             SetConfig();
             return true;
         }
         return false;
    }
    void view() { ImGui::Plot("plot", conf); }
    void SetConfig()
    {
        //conf.values.xs = x_data; // this line is optional
        //conf.values.ys = (float*)robotica::websocket::instance().audio_buffer[0];
        conf.values.count = audioFile.getNumSamplesPerChannel();
        conf.scale.min = -1;
        conf.scale.max = 1;
        conf.tooltip.show = true;
        conf.tooltip.format = "x=%.2f, y=%.2f";
        conf.grid_x.show = true;
        conf.grid_y.show = true;
        conf.frame_size = ImVec2(400, 400);
        conf.line_thickness = 2.f;
    }
    void SetData()
    {
        int channel = audioFile.getNumChannels();
        const int numSamples = audioFile.getNumSamplesPerChannel();
        for (int j = 0; j < numSamples; j++)
        {
            for (int i = 0; i < channel; i++)
            {
                double currentSample = audioFile.samples[0][j];
                data.push_back(currentSample);
            }
        }
    }
    void PrintSum()
    {
        audioFile.printSummary();
    }
};
