#pragma once
#include "dance/AudioFile.h"
#include "window/imgui_plot/imgui_plot.hpp"

class MusicAnalyzer
{
    AudioFile<double> audioFile;
    std::vector<float> data;
    ImGui::PlotConfig conf;
  public:
    bool loadfile() { return audioFile.load("E:/Robotica/music/wariors.wav"); }

    void SetConfig()
    {
        //conf.values.xs = x_data; // this line is optional
        conf.values.ys = &data.front();
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
    void getSampels()
    {
        int channel = audioFile.getNumChannels();
        const int numSamples = audioFile.getNumSamplesPerChannel();
        for (int j = 0; j < numSamples; j++)
        {
            for (int i = 0; i < channel; i++)
            {
                double currentSample = audioFile.samples[i][j];
                data.push_back(currentSample);
            }
        }
    }
};
