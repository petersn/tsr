all: psr spectrogram

# You must define one of the following two:
#  -DUSE_FLOAT          -- use floats as the internal audio representation instead of doubles
#  -DUSE_DOUBLE         -- use doubles as the internal audio representation instead of floats

# Optional arguments to these libraries:
#  -DQUIT_ON_WARNING    -- exit(-1) after every warning

psr: audio.c audio.h fourier.c fourier.h microphone.c microphone.h features.c features.h production.c production.h main.c Makefile
	gcc -O3 -lm -DUSE_FLOAT -DQUIT_ON_WARNING main.c audio.c fourier.c microphone.c features.c production.c -Wall -o psr

# Optional arguments to the spectrogram program:
#  -DSQUARE_SUPPRESS    -- filter out low intensity frequencies by squaring every component
#  -DBARTLET_WINDOWING  -- apply a Bartlett window to each slice
#  -DFREQ_MULT=n        -- Hertz per vertical pixel in spectrogram
#  -DMS_IN_TIME_SLICE=n -- milliseconds per horizontal pixel in spectrogram
#  -DX_STRETCH=n        -- horizontal pixels per slice in spectrogram
#  -DY_STRETCH=n        -- vertical pixesl per frequency bin in spectrogram
#  -DNO_FULLSCREEN      -- don't produce a fullscreen window
#  -DNO_HWSURFACE       -- don't attempt to use hardware accelerated surfaces
#  -DFORCE_WIDTH=n      -- force a window width
#  -DFORCE_HEIGHT=n     -- force a window height

spectrogram: audio.c audio.h fourier.c fourier.h microphone.c microphone.h spectrogram.c Makefile
	gcc -O3 -lm -DUSE_FLOAT -DQUIT_ON_WARNING spectrogram.c audio.c fourier.c microphone.c -Wall -o gram `sdl-config --cflags --libs`

