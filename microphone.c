 // Microphone reading library
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/soundcard.h>

#include "audio.h"
#include "microphone.h"

int audio_device;

void microphone_init() {
    audio_device = open("/dev/dsp", O_RDONLY);

    //*
    int tmp;

      tmp = AFMT_S16_NE;		// Native 16 bits
  if (ioctl (audio_device, SNDCTL_DSP_SETFMT, &tmp) == -1)
    {
      perror ("SNDCTL_DSP_SETFMT");
      exit (-1);
    }

  if (tmp != AFMT_S16_NE)
    {
      fprintf (stderr,
	       "The device doesn't support the 16 bit sample format.\n");
      exit (-1);
    }

    tmp = 1;
  if (ioctl (audio_device, SNDCTL_DSP_CHANNELS, &tmp) == -1)
    {
      perror ("SNDCTL_DSP_CHANNELS");
      exit (-1);
    }

  if (tmp != 1)
    {
      fprintf (stderr, "The device doesn't support mono mode.\n");
      exit (-1);
    }

    int sample_rate = MIC_SAMP_RATE;
  if (ioctl (audio_device, SNDCTL_DSP_SPEED, &sample_rate) == -1)
    {
      perror ("SNDCTL_DSP_SPEED");
      exit (-1);
    }
    //*/

}

void microphone_deinit() {
    close( audio_device );
}

size_t microphone_read( size_t samples, Audio *buffer ) {
    if (buffer == NULL) {
        printf("Warning: Attempted to read microphone into a NULL buffer.\n");
#ifdef QUIT_ON_WARNING
        exit(-1);
#endif
        return 0;
    }
    if (buffer->format != MIC_FORMAT) {
        printf("Warning: Attempt to read format %i from microphone into a buffer of format %i.\n", MIC_FORMAT, buffer->format);
#ifdef QUIT_ON_WARNING
        exit(-1);
#endif
        return 0;
    }
    if (buffer->samples < samples) {
        printf("Warning: Attempt to read %ld microphone samples into a buffer of length %ld.\n", samples, buffer->samples);
#ifdef QUIT_ON_WARNING
        exit(-1);
#endif
        return 0;
    }

    return read( audio_device, buffer->data, samples * MIC_SAMPLE_BYTES );

}

