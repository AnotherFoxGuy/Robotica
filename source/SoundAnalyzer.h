#pragma once
#include <iostream>
#include "minimp3.h"
#include "minimp3_ex.h"

class SoundAnalyzer
{
public:


	typedef struct decoder
    {
        mp3dec_ex_t mp3d;
        float mp3_duration;
        float spectrum[32][2]; // for visualization
    } decoder;



};
