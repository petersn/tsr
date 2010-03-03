 // Audio types library
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "audio.h"

const char *format_names[MAX_AUDIO_FORMAT] = {
    "NO_FORMAT",
    "EIGHT_BIT_UNSIGNED",
    "SIXTEEN_BIT_SIGNED",
    "DOUBLE_REAL",
    };

size_t sample_size[MAX_AUDIO_FORMAT] = {
        0,
        sizeof( char ),
        sizeof( short ),
        sizeof( Real ),
    };

void audio_free( Audio *audio ) {
    if (audio == NULL) return;

    free(audio->data);
    free(audio);
    //audio->format = NO_FORMAT;

}

Audio *audio_duplicate( Audio *audio ) {
    if (audio == NULL) return NULL;

    Audio *new;
    new = malloc( sizeof( Audio ) );

    size_t total_bytes = sample_size[audio->format] * audio->samples;

    new->format = audio->format;
    new->data   = malloc( total_bytes );
    memcpy( new->data, audio->data, total_bytes );

    return new;
}

void audio_sum( Audio *audio, Audio *budio ) {
    if (audio == NULL || budio == NULL) return;

    int ii, max;
    Real *auffer, *buffer;

    auffer = (Real *)audio->data;
    buffer = (Real *)budio->data;

    max = audio->samples;
    if (budio->samples < max)
        max = budio->samples;

    for (ii=0; ii<max; ii++) {
        buffer[ii] += auffer[ii];
    }

}

void audio_scale( Audio *audio, Real scaling ) {
    if (audio == NULL) return;

    int ii;
    Real *auffer;

    auffer = (Real *)audio->data;

    for (ii=0; ii<audio->samples; ii++) {
        auffer[ii] *= scaling;
    }

}

Audio *audio_make_buffer( size_t samples, Audio_Format format ) {
    Audio *new;
    void *buffer;

    buffer = malloc( sample_size[format] * samples );

    new = malloc( sizeof( Audio ) );
    new->format  = format;
    new->data    = buffer;
    new->samples = samples;

    return new;
}

void audio_convert( Audio *audio, Audio_Format format ) {
    if (audio == NULL) return;
    if (audio->format == format) return;

    int ii;

      // If the current format is NO_FORMAT, then set an empty buffer
    if (audio->format == NO_FORMAT) {
        audio->samples = 0;
        audio->data    = malloc( 0 );
        audio->format  = format;
        return;
    }

    size_t buffer_length = sample_size[format] * audio->samples;

      // N^2 different ways to cast
    switch (format) {
        case NO_FORMAT:
            free(audio->data);
            audio->samples = 0;
            audio->data    = NULL;
            break;

        case DOUBLE_REAL: {
            Real *buffer = malloc( buffer_length );
            switch (audio->format) {
                case EIGHT_BIT_UNSIGNED: {
                    for (ii=0; ii<audio->samples; ii++) {
                        buffer[ii] = ((unsigned char *)audio->data)[ii];
                    }
                    break;
                }
                case SIXTEEN_BIT_SIGNED: {
                    for (ii=0; ii<audio->samples; ii++) {
                        buffer[ii] = ((signed short *)audio->data)[ii];
                    }
                    break;
                }
                default:
                    printf("Warning: No support to cast to DOUBLE_REAL from format %s\n", format_names[audio->format]);
#ifdef QUIT_ON_WARNING
                    exit(-1);
#endif
                    return;
            }
            free(audio->data);
            audio->data = (void *)buffer;
            break;
        }

        default:
            printf("Warning: No support to cast to format %s\n", format_names[format]);
#ifdef QUIT_ON_WARNING
            exit(-1);
#endif
            return;
    }

    audio->format = format;

}

