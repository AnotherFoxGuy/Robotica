#include "dance/MusicAnalyzer.hpp"
MusicAnalyzer& MusicAnalyzer::instance()
{
    static MusicAnalyzer i;
    return i;
}
