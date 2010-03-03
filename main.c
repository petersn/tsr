 // Main frontend for PSR
#include <stdio.h>
#include <stdlib.h>

#include "audio.h"
#include "fourier.h"
#include "microphone.h"
#include "features.h"

#define MS_IN_TIME_SLICE 50

#define SAMPLES_IN_TIME_SLICE  ((size_t)( ((double)MIC_SAMP_RATE) / ( ((double)1000.0) / ((double)MS_IN_TIME_SLICE) )))

int main(int argc, char **argv) {
    Audio    *audio_input = NULL;

    printf("PSR2 Speech Recognizer\n");

    /*
    printf("Formats:\nNO_FORMAT: %i\nEIGHT_BIT_UNSIGNED: %i\nSIXTEEN_BIT_SIGNED: %i\nDOUBLE_REAL: %i\n\n", NO_FORMAT, EIGHT_BIT_UNSIGNED, SIXTEEN_BIT_SIGNED, DOUBLE_REAL);

    printf("Samples in slice: %ld\n\n", SAMPLES_IN_TIME_SLICE);
    */

      // Initialize
    microphone_init();

    audio_input = NULL;
    //frequencies = audio_make_buffer( SAMPLES_IN_TIME_SLICE, DOUBLE_REAL );

    size_t read_samples;

    while (1) {

        audio_free( audio_input );
        audio_input = audio_make_buffer( SAMPLES_IN_TIME_SLICE, MIC_FORMAT );

          // Read in 50 ms of audio
        read_samples = microphone_read( SAMPLES_IN_TIME_SLICE, audio_input );
        //printf("Samples: %ld\n", read_samples);

        audio_convert( audio_input, DOUBLE_REAL );

        //features = features_extract( audio_input, NULL );

        //fourier_transform( audio_input, frequencies, SAMPLES_IN_TIME_SLICE );

    }

      // Clean up
    microphone_deinit();

    return 0;

}

