 // Fourier transform library header

#ifndef TSR_FOURIER_HEADER
#define TSR_FOURIER_HEADER

#include "audio.h"

typedef Audio Fourier;

Real frequency_component( Audio *audio, Real freq );
void fourier_transform( Audio *audio, Fourier *result, size_t samples_in_one_second );
void bartlett_window( Audio *audio );

#endif

