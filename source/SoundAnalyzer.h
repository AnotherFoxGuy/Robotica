#pragma once
#include "minimp3.h"
#include "minimp3_ex.h"
#include <iostream>

class SoundAnalyzer
{
    typedef struct decoder
    {
        mp3dec_ex_t mp3d;
        float mp3_duration;
        float spectrum[32][2]; // for visualization
    } decoder;

  public:
    void scan_mp3()
    {
        static mp3dec_ex_t mp3d;
        // mp3dec_init()
        //mp3dec_file_info_t info;
        if (mp3dec_ex_open(&mp3d, "Rasputin.mp3", MP3D_SEEK_TO_SAMPLE))
        {
            /* error */
        }

        auto y = mp3d.info.hz;

        /* dec.samples, dec.info.hz, dec.info.layer, dec.info.channels should be filled */
        if (mp3dec_ex_seek(&mp3d, 1))
        {
            /* error */
        }

        mp3d_sample_t* buffer = (mp3d_sample_t*)malloc(mp3d.samples * sizeof(mp3d_sample_t));
        size_t readed = mp3dec_ex_read(&mp3d, buffer, mp3d.samples);
        if (readed != mp3d.samples) /* normal eof or error condition */
        {
            if (mp3d.last_error)
            {
                /* error */
                int x = 0;
            }
        }

        ////for (size_t i = 0; i < info.samples; i++)
        ////{
        ////    std::cout << info.buffer[i] << "|";
        ////}
        ////std::cout << info.hz << std::endl;
        
        decoder *x , d;
        x = &d;
        (*x).mp3_duration = 30;
        (*x).mp3d = mp3d;
        get_spectrum(x, mp3d.info.channels);
    }

    static void get_spectrum(decoder* dec, int numch)
    {
        int i, ch, band;
        const mp3dec_t* d = &dec->mp3d.mp3d;
        // Average spectrum power for 32 frequency band
        for (ch = 0; ch < numch; ch++)
        {
            for (band = 0; band < 32; band++)
            {
                float band_power = 0;
                for (i = 0; i < 9; i++)
                {
                    float sample = d->mdct_overlap[ch][i + band * 9];
                    band_power += sample * sample;
                }
                // with averaging
                //dec->spectrum[band][ch] += (band_power/9 - spectrum[band][ch]) * 0.25;

                // w/o averaging
                dec->spectrum[band][ch] = band_power / 9;
            }
        }
        // Calculate dB scale from power for better visualization
        for (ch = 0; ch < numch; ch++)
        {
            for (band = 0; band < 32; band++)
            {
                float power = dec->spectrum[band][ch];
                float db_offset = 100; // to shift dB values from [0..-inf] to [max..0]
                float db = 10 * log10f(power + 1e-15) + db_offset;
                if (db < 0) db = 0;
                printf("% .5f\t", db);
            }
            printf("\n");
        }
    }
};
