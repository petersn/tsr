 // Fourier transform library
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI ((Real)(3.14159265358979323846))
#endif

#include "audio.h"
#include "fourier.h"

Real frequency_component( Audio *audio, Real freq ) {
    if (audio == NULL) {
        printf("Warning: Attempted Fourier multiplication on NULL audio.\n");
#ifdef QUIT_ON_WARNING
        exit(-1);
#endif
        return 0.0;
    }
    if (audio->format != DOUBLE_REAL) {
        printf("Warning: Attempted Fourier multiplication on audio of format %s.\n", format_names[audio->format]);
#ifdef QUIT_ON_WARNING
        exit(-1);
#endif
        return 0.0;
    }

    int ii;
    Real accumulate, wave;

    accumulate = 0;

    for (ii=0; ii<audio->samples; ii++) {
        wave = cos( 2.0 * M_PI * ((Real) ii) * freq );
        accumulate += ((Real *)audio->data)[ii] * wave;
    }

    return accumulate;

}

void bartlett_window( Audio *audio ) {
    if (audio == NULL) {
        printf("Warning: Attempted Bartlett windowing on NULL audio.\n");
#ifdef QUIT_ON_WARNING
        exit(-1);
#endif
        return;
    }
    if (audio->format != DOUBLE_REAL) {
        printf("Warning: Attempted Bartlett windowing on audio of format %s.\n", format_names[audio->format]);
#ifdef QUIT_ON_WARNING
        exit(-1);
#endif
        return;
    }

    Real  coef;
    Real *data;
    int ii;

    data = (Real *)audio->data;

      //Rising edge of the window
    for (ii=0; ii<(audio->samples/2); ii++) {
        coef = ((Real)ii) / ((Real)audio->samples/2);
        data[ii] *= coef;
    }
      //Falling edge of the window
    while (ii<(audio->samples)) {
        coef = ((Real)(ii-(audio->samples/2))) / ((Real)audio->samples/2);
        data[ii] *= coef;
        ii++;
    }

}

Real alpha[4] = {
                    0.35875,
                    0.48829,
                    0.14128,
                    0.01168,
                };

void blackman_harris_window( Audio *audio ) {
    if (audio == NULL) {
        printf("Warning: Attempted Blackman-Harris windowing on NULL audio.\n");
#ifdef QUIT_ON_WARNING
        exit(-1);
#endif
        return;
    }
    if (audio->format != DOUBLE_REAL) {
        printf("Warning: Attempted Blackman-Harris windowing on audio of format %s.\n", format_names[audio->format]);
#ifdef QUIT_ON_WARNING
        exit(-1);
#endif
        return;
    }

    Real  coef;
    Real *data;
    int ii;
    int N = audio->samples;

    data = (Real *)audio->data;

      //Windowing
    for (ii=0; ii<N; ii++) {
        coef = alpha[0] - alpha[1] * cos( ((Real)ii * 2 * M_PI) / ((Real) N-1 ) ) \
                        + alpha[2] * cos( ((Real)ii * 4 * M_PI) / ((Real) N-1 ) ) \
                        - alpha[3] * cos( ((Real)ii * 6 * M_PI) / ((Real) N-1 ) );

        data[ii] *= coef;
    }

}

void hann_window( Audio *audio ) {
    if (audio == NULL) {
        printf("Warning: Attempted Hann windowing on NULL audio.\n");
#ifdef QUIT_ON_WARNING
        exit(-1);
#endif
        return;
    }
    if (audio->format != DOUBLE_REAL) {
        printf("Warning: Attempted Hann windowing on audio of format %s.\n", format_names[audio->format]);
#ifdef QUIT_ON_WARNING
        exit(-1);
#endif
        return;
    }

    Real  coef;
    Real *data;
    int ii;
    int N = audio->samples;

    data = (Real *)audio->data;

      //Windowing
    for (ii=0; ii<N; ii++) {
        coef = 0.5 * (1.0 - cos ( ( 2.0 * M_PI * (Real)ii ) / ( N-1 ) ));
        data[ii] *= coef;
    }

}

inline Real rejigger( Real v ) {
    if (v < 0.0) {
        return -v;
    }
    return v;
}

void normalize( Audio *audio ) {
    if (audio == NULL) {
        printf("Warning: Attempted to normalize NULL audio.\n");
#ifdef QUIT_ON_WARNING
        exit(-1);
#endif
        return;
    }
    if (audio->format != DOUBLE_REAL) {
        printf("Warning: Attempted to normalize audio of format %s.\n", format_names[audio->format]);
#ifdef QUIT_ON_WARNING
        exit(-1);
#endif
        return;
    }
    int ii;
    Real accumulate = 0.0;

      // First, find the average
    for (ii=0; ii<audio->samples; ii++) {
        accumulate += rejigger(((Real *)audio->data)[ii]);
    }
    accumulate /= (Real)audio->samples;

    //printf("Mean: %f\n", accumulate);

      // Next, divide each sample by the average
    accumulate = 1.0 / accumulate;

    for (ii=0; ii<audio->samples; ii++) {
        ((Real*)audio->data)[ii] *= accumulate;
    }

}

void fourier_transform( Audio *audio, Fourier *result, size_t samples_in_one_second ) {
    if (audio == NULL) {
        printf("Warning: Attempted Fourier transform on NULL audio.\n");
#ifdef QUIT_ON_WARNING
        exit(-1);
#endif
        return;
    }
    if (result == NULL) {
        printf("Warning: Attempted Fourier transform into NULL result buffer.\n");
#ifdef QUIT_ON_WARNING
        exit(-1);
#endif
        return;
    }
    if (audio->format != DOUBLE_REAL) {
        printf("Warning: Attempted Fourier transform on audio of format %s.\n", format_names[audio->format]);
#ifdef QUIT_ON_WARNING
        exit(-1);
#endif
        return;
    }
    if (result->format != DOUBLE_REAL) {
        printf("Warning: Attempted Fourier transform into a buffer of format %s.\n", format_names[audio->format]);
#ifdef QUIT_ON_WARNING
        exit(-1);
#endif
        return;
    }
    if (audio->samples != result->samples) {
        printf("Warning: Attempted Fourier transform on %ld samples into a buffer of %ld samples.\n", audio->samples, result->samples);
#ifdef QUIT_ON_WARNING
        exit(-1);
#endif
        return;
    }

    int ii;
    Real sios = (Real)samples_in_one_second;

      // Begin complex matrix multiplication
    for (ii=0; ii<result->samples; ii++) {

        ((Real *)result->data)[ii] = frequency_component( audio, ((Real)ii)/sios );

    }

}

