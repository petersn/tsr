all: tsr spectrogram

CC=gcc
CFLAGS=-O3 -lm -Wall
OPTIONS=

# You must define one of the following two:
OPTIONS+=-DUSE_FLOAT#             #-- use floats as the internal audio representation instead of doubles
OPTIONS+=-DUSE_DOUBLE#            #-- use doubles as the internal audio representation instead of floats

# Optional arguments to these libraries:
OPTIONS+=-DQUIT_ON_WARNING#       #-- exit(-1) after every warning

# You may apply one (more than one allowed, but not adviced) window to reduce spectral leakage
#OPTIONS+=-DBARTLETT_WINDOW#       #-- apply a Bartlett window to each slice (cheap, low leakage rolloff)
OPTIONS+=-DBLACKMAN_HARRIS_WINDOW##-- apply a Blackman-Harris window to each slice (expensive, high leakage rolloff)

# Optional arguments to the spectrogram program:
#OPTIONS+=-DSQUARE_SUPPRESS#       #-- filter out low intensity frequencies by squaring every component

#OPTIONS+=-DFREQ_MULT=n#           #-- Hertz per vertical pixel in spectrogram
#OPTIONS+=-DMS_IN_TIME_SLICE=n#    #-- milliseconds per horizontal pixel in spectrogram
#OPTIONS+=-DX_STRETCH=n#           #-- horizontal pixels per slice in spectrogram
#OPTIONS+=-DY_STRETCH=n#           #-- vertical pixesl per frequency bin in spectrogram
#OPTIONS+=-DNO_FULLSCREEN#         #-- don't produce a fullscreen window
#OPTIONS+=-DNO_HWSURFACE#          #-- don't attempt to use hardware accelerated surfaces
#OPTIONS+=-DFORCE_WIDTH=n#         #-- force a window width
#OPTIONS+=-DFORCE_HEIGHT=n#        #-- force a window height

tsr: audio.c audio.h fourier.c fourier.h microphone.c microphone.h features.c features.h production.c production.h main.c Makefile
	$(CC) $(CFLAGS) $(OPTIONS) main.c audio.c fourier.c microphone.c features.c production.c -o tsr

spectrogram: audio.c audio.h fourier.c fourier.h microphone.c microphone.h spectrogram.c Makefile
	$(CC) $(CFLAGS) $(OPTIONS) spectrogram.c audio.c fourier.c microphone.c -o gram `sdl-config --cflags --libs`
