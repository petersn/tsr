 // Main frontend for TSR
#include <stdio.h>
#include <stdlib.h>

#include "audio.h"
#include "fourier.h"
#include "microphone.h"
#include "features.h"
#include "dynamic_programming.h"

#ifndef RUNNING_AVERAGE_TAU
#define RUNNING_AVERAGE_TAU 0.9
#endif

#ifndef MS_IN_TIME_SLICE
#define MS_IN_TIME_SLICE 50
#endif

#define SAMPLES_IN_TIME_SLICE  ((size_t)( ((double)MIC_SAMP_RATE) / ( ((double)1000.0) / ((double)MS_IN_TIME_SLICE) )))

int main(int argc, char **argv) {
    Audio    *audio_input = NULL;
    Features features;

#ifdef SUB_RUNNING_AVERAGE
    Features running_average;
    Features features_temporary;
    zero_features( &running_average );
#endif

    printf("Tiny Speech Recognizer\n");

      // Initialize
    microphone_init();

    audio_input = NULL;

    size_t read_samples;

    while (1) {

        audio_free( audio_input );
        audio_input = audio_make_buffer( SAMPLES_IN_TIME_SLICE, MIC_FORMAT );

          // Read in a slice of audio
        read_samples = microphone_read( SAMPLES_IN_TIME_SLICE, audio_input );

        audio_convert( audio_input, DOUBLE_REAL );

        features = features_extract( audio_input );

#ifdef SUB_RUNNING_AVERAGE
          // First, copy the current feature vector, weighted by tau
          // We copy these before applying the running average subtraction to get them untainted
        copy_scaled_features( &features_temporary, &features, ((Real)1.0-(Real)RUNNING_AVERAGE_TAU) );
          // Apply the running average subtraction to the current feature vector now that we've saved it
        subtract_features( &features, &running_average );
          // Next, scale the current running average down a little bit
        scale_features( &running_average, (Real)RUNNING_AVERAGE_TAU );
          // Add the saved scaled features into running_average
        add_features( &running_average, &features_temporary );
#endif

#ifdef SQUARE_SUPPRESS
        square_suppress( &features );
#endif

        if (above_threshold( &features )) {
            features_pretty( features );
        }

        //fourier_transform( audio_input, frequencies, SAMPLES_IN_TIME_SLICE );

    }

      // Clean up
    microphone_deinit();

    return 0;

}

