all: tsr spectrogram

CC=gcc
CFLAGS=-lm -Wall
OPTIONS=

  # You must define exactly one of the following two:
#OPTIONS+=-DUSE_FLOAT#             #-- use floats as the internal audio representation instead of doubles
OPTIONS+=-DUSE_DOUBLE#            #-- use doubles as the internal audio representation instead of floats

  # Optional arguments to these libraries:
OPTIONS+=-DQUIT_ON_WARNING#       #-- exit(-1) after every warning

  # You may apply one (more than one allowed, but not adviced) window to reduce spectral leakage
#OPTIONS+=-DBARTLETT_WINDOW#       #-- apply a Bartlett window to each slice (cheap, low leakage rolloff)
#OPTIONS+=-DBLACKMAN_HARRIS_WINDOW##-- apply a Blackman-Harris window to each slice (expensive, high leakage rolloff)
OPTIONS+=-DHANN_WINDOW#           #-- apply a Hann window to each slice (minimal Gibbs phenomenon)

  # Recognition options
#OPTIONS+=-DUTTERANCE_THRESHOLD=n# #-- energy required to begin recognizing an utterance

  # Optional arguments to the spectrogram program:
OPTIONS+=-DSQUARE_SUPPRESS#       #-- filter out low intensity frequencies by squaring every component
#OPTIONS+=-DFORMANT_AMP#           #-- multiply every feature by its frequency to amplify formants
#OPTIONS+=-DNORMALIZE_FOURIER#     #-- normalize all fourier vectors

#OPTIONS+=-DFREQ_MULT=n#           #-- Hertz per vertical pixel in spectrogram
OPTIONS+=-DMS_IN_TIME_SLICE=10#    #-- milliseconds per horizontal pixel in spectrogram
#OPTIONS+=-DX_STRETCH=n#           #-- horizontal pixels per slice in spectrogram
#OPTIONS+=-DY_STRETCH=n#           #-- vertical pixesl per frequency bin in spectrogram
#OPTIONS+=-DNO_FULLSCREEN#         #-- don't produce a fullscreen window
#OPTIONS+=-DNO_HWSURFACE#          #-- don't attempt to use hardware accelerated surfaces
#OPTIONS+=-DFORCE_WIDTH=800#         #-- force a window width
#OPTIONS+=-DFORCE_HEIGHT=400#        #-- force a window height

 # Optional arguments to TSR:
#OPTIONS+=-DSUB_RUNNING_AVERAGE#   #-- subtract the running average feature vector (excludes constant noise)
#OPTIONS+=-DRUNNING_AVERAGE_TAU=x# #-- time constant for the running average (0.9 averages very slowly, 0.1 keeps almost no history)

  # Arguments to control affine frequency sampling function for feature vectors
#OPTIONS+=-DBUCKET_COUNT=n#        #-- set the number of buckets in a feature vector
#OPTIONS+=-DSAMPLING_MIN_FREQ=n#   #-- set the minimum frequency sampled by the feature extractor
#OPTIONS+=-DSAMPLING_MAX_FREQ=n#   #-- set the maximum frequency sampled by the feature extractor

  # Optimization settings
 # Speed
OPTIONS+=-O3
 # Size
#OPTIONS+=-Os

  # Premixed configurations. Uncomment one to override all above config.
 # Tiny, fast build
#OPTIONS=-Os -DUSE_FLOAT -DQUIT_ON_WARNING
 # Bloated, featureful build
#OPTIONS=-O3 -DUSE_DOUBLE -DQUIT_ON_WARNING -DHANN_WINDOW -DSUB_RUNNING_AVERAGE -DSQUARE_SUPPRESS

tsr: audio.c audio.h fourier.c fourier.h microphone.c microphone.h features.c features.h dynamic_programming.c dynamic_programming.h main.c Makefile
	$(CC) $(CFLAGS) $(OPTIONS) main.c audio.c fourier.c microphone.c features.c production.c dynamic_programming.c -o tsr

spectrogram: audio.c audio.h fourier.c fourier.h microphone.c microphone.h spectrogram.c Makefile
	$(CC) $(CFLAGS) $(OPTIONS) spectrogram.c audio.c fourier.c microphone.c -o gram `sdl-config --cflags --libs`

