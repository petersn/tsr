 // Fourier transform library header

#ifndef PSR_FOURIER_HEADER
#define PSR_FOURIER_HEADER

#include "audio.h"

typedef Audio Fourier;

Real frequency_component( Audio *audio, Real freq );
void fourier_transform( Audio *audio, Fourier *result, size_t samples_in_one_second );
void bartlett_window( Audio *audio );

#endif

