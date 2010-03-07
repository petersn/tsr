 // Feature extractor library
#include <stdio.h>
#include <stdlib.h>

#include "audio.h"
#include "features.h"
#include "fourier.h"
#include "microphone.h"

/*
double key_frequencies[BUCKET_COUNT] = {
        100.0,
        120.0,
        140.0,
        160.0,
        180.0,
    };
*/

inline Real key_frequency(int index) {
    return (Real)AFFINE_SAMPLING_ADD + (Real)index * (Real)AFFINE_SAMPLING_MULT;
}

Features features_extract( Audio *audio ) {
    Features result;
    int ii;

    for (ii=0; ii<BUCKET_COUNT; ii++) {
        result.buckets[ii] = frequency_component( audio, frequency( key_frequency(ii) ) );
        if (result.buckets[ii] < 0.0)
            result.buckets[ii] = -result.buckets[ii];
    }

    return result;

}

void square_suppress( Features *features ) {
    int ii;

    for (ii=0; ii<BUCKET_COUNT; ii++) {
        features->buckets[ii] *= 0.001;
        features->buckets[ii] *= features->buckets[ii];
    }
}

int above_threshold( Features *features ) {
    Real accumulate = 0.0;
    int ii;

    for (ii=0; ii<BUCKET_COUNT; ii++) {
        accumulate += features->buckets[ii];
    }

    if (accumulate < NORMALIZED_UTTERANCE_THRESHOLD)
        printf("%f accumulated, threshold %f\n", accumulate, NORMALIZED_UTTERANCE_THRESHOLD);

    return (accumulate > NORMALIZED_UTTERANCE_THRESHOLD);
}

void features_pretty( Features features ) {
    int ii;

    printf("{ ");
    for (ii=0; ii<BUCKET_COUNT; ii++) {
        printf("%4.0f ", features.buckets[ii]*0.0001);
    }
    printf("}\n");

}

void zero_features( Features *features ) {
    int ii;

    for (ii=0; ii<BUCKET_COUNT; ii++) {
        features->buckets[ii] = (Real)0.0;
    }

}

void scale_features( Features *features, Real scalar ) {
    int ii;

    for (ii=0; ii<BUCKET_COUNT; ii++) {
        features->buckets[ii] *= scalar;
    }

}

void copy_scaled_features( Features *features, Features *beatures, Real scalar ) {
    int ii;

    for (ii=0; ii<BUCKET_COUNT; ii++) {
        features->buckets[ii] = beatures->buckets[ii] * scalar;
    }

}

void add_features( Features *features, Features *beatures ) {
    int ii;

    for (ii=0; ii<BUCKET_COUNT; ii++) {
        features->buckets[ii] += beatures->buckets[ii];
    }

}

void subtract_features( Features *features, Features *beatures ) {
    int ii;

    for (ii=0; ii<BUCKET_COUNT; ii++) {
        features->buckets[ii] -= beatures->buckets[ii];
    }

}

