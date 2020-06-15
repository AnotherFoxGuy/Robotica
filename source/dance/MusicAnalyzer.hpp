#pragma once
#include "dance/AudioFile.h"
class MusicAnalyzer
{
    AudioFile<double> audioFile;

  public:
    bool loadfile() { return audioFile.load("E:/Robotica/music/wariors.wav"); }

    void printInfo() { audioFile.printSummary(); }
    void getSampels()
    {
        double prefiousSamples = 0;
        bool goingDown = true;
        int channel = audioFile.getNumChannels();
        int hz[2]{0, 0};
        int maxhz = 0;
        int minhz = 10000;
        const int numSamples = audioFile.getNumSamplesPerChannel();
        for (int j = 0; j < numSamples; j++)
        {
            for (int i = 0; i < channel; i++)
            {
                double currentSample = audioFile.samples[i][j];
                // printf("sample %f \n", currentSample);
                if (prefiousSamples > currentSample && currentSample > 0 && goingDown)
                {
                    hz[i]++;
                    goingDown = false;
                }
                if (prefiousSamples < 0 && currentSample > 0)
                    goingDown = true;

                prefiousSamples = currentSample;
                //printf("%f \n", currentSample);
            }
            if (j % audioFile.getSampleRate() == 0 && j != 0)
            {
                for (int i = 0; i < audioFile.getNumChannels(); i++)
                {
                    printf("hz[%d]: %d ", i, hz[i]);
                    if (hz[i] > maxhz)
                        maxhz = hz[i];
                    if (hz[i] < minhz)
                        minhz = hz[i];

                    hz[i] = 0;
                }
                printf("\n");
            }
        }
        printf("minhz: %d maxhz: %d", minhz, maxhz);
    }
};
