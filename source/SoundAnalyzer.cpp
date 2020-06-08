#include "SoundAnalyzer.hpp"


void SoundAnalyzer::scan_mp3()
{
    open_dec(&_dec, "Rasputin.mp3");
    get_spectrum(&_dec, _dec.mp3d.info.channels);
}
