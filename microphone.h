 // Microphone reading library header

#ifndef PSR_MICROPHONE_HEADER
#define PSR_MICROPHONE_HEADER

#include "audio.h"

#define MIC_SAMP_RATE    48000
#define MIC_SAMPLE_BYTES 2
#define MIC_FORMAT       SIXTEEN_BIT_SIGNED

#define frequency(x)    ((x)/((Real)MIC_SAMP_RATE))

  // Microphone functions:
void microphone_init();
void microphone_deinit();

size_t microphone_read( size_t samples, Audio *buffer );

#endif

