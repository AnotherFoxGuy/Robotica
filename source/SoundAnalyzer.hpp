#pragma once
#include "minimp3.h"
#include "minimp3_ex.h"
#include "decode.h"
#include <iostream>

class SoundAnalyzer
{
public:

    decoder _dec;

	void scan_mp3();
};
