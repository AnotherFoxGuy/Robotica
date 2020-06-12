#pragma once
#include "dance/AudioFile.h"
class MusicAnalyzer
{
    AudioFile<double> audioFile;
public:
    bool loadfile() { return audioFile.load("E:/Robotica/kaas.wav");}
    
    void printInfo() { audioFile.printSummary();}
    void getSampels()
    {
        int channel = audioFile.getNumChannels();
        int numSamples = audioFile.getNumSamplesPerChannel();
        for (int j = 0; j < channel; j++)
        {   
            for (int i = 0; i < numSamples; i++)
            {
                double currentSample = audioFile.samples[j][i];
                std::cout << currentSample << std::endl;
            }
        }
    }
};
