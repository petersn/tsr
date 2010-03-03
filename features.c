 // Feature extractor library
#include <stdio.h>
#include <stdlib.h>

#include "audio.h"
#include "features.h"
#include "fourier.h"
#include "microphone.h"

double key_frequencies[BUCKET_COUNT] = {
    100.0,
    };

Features features_extract( Audio *audio, double *key_frequencies ) {
    Features result;
    int ii;

    for (ii=0; ii<BUCKET_COUNT; ii++) {
        result.buckets[ii] = frequency_component( audio, frequency( key_frequencies[ii] ) );
    }

    return result;

}

