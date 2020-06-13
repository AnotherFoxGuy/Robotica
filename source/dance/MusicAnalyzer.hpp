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
        double avrageSamples = 0;
        int length = 0;
        int channel = 0; //audioFile.getNumChannels();
        int numSamples = audioFile.getNumSamplesPerChannel();
        for (int j = 0; j < numSamples; j++)
        {
            //for (int i = 0; i < channel; i++)
            //{
                double currentSample = audioFile.samples[channel][j];
                avrageSamples += currentSample * audioFile.getBitDepth();
      
            //}
            if (j % audioFile.getSampleRate() == 0 && j != 0)
            {
                int sec = j/ audioFile.getSampleRate();
                
                printf("sample %f sec: %d \n", (avrageSamples / audioFile.getSampleRate()) * audioFile.getSampleRate(),sec);
                avrageSamples = 0;
            }
            

        }
    }
};
