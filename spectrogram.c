 // Spectrogram test example for audio.c, fourier.c, and microphone.c
#include <stdio.h>
#include <SDL.h>

#include "audio.h"
#include "fourier.h"
#include "microphone.h"

int WIDTH;
int HEIGHT;
#define BPP 4
#define DEPTH 32

#ifndef X_STRETCH
#define X_STRETCH 4
#endif

#ifndef Y_STRETCH
#define Y_STRETCH 4
#endif

#ifndef FREQ_MULT
#define FREQ_MULT ((double) 5.0 )
#endif

#ifndef MS_IN_TIME_SLICE
#define MS_IN_TIME_SLICE 50
#endif

#define SAMPLES_IN_TIME_SLICE  ((size_t)( ((double)MIC_SAMP_RATE) / ( ((double)1000.0) / ((double)MS_IN_TIME_SLICE) )))

void setpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    Uint32 *pixmem32;
    Uint32 colour;

    colour = SDL_MapRGB( screen->format, r, g, b );

    int adjusted_y;

    adjusted_y = y*screen->pitch/BPP;

    pixmem32 = (Uint32*) screen->pixels + adjusted_y + x;
    *pixmem32 = colour;
}

void DrawScreen(SDL_Surface* screen, Audio *audio, int xx)
{

    if(SDL_MUSTLOCK(screen))
    {
        if(SDL_LockSurface(screen) < 0) return;
    }

    int ii, xs, ys;
    Real value;
    int red, green, blue;

    for (ii=0; ii<(HEIGHT/Y_STRETCH); ii++) {
        //value = frequency_component( audio, frequency( 500.0 + (((double)ii) * FREQ_MULT) ) );
        value = frequency_component( audio, frequency( (((double)(ii*Y_STRETCH)) * FREQ_MULT) ) );

        //printf("%f\n", value);

#ifdef SQUARE_SUPPRESS
        value *= 0.001;
        value *= value;
#endif

        value *= 0.001;

#ifndef SQUARE_SUPPRESS
        if (value < 0) value = -value;
#endif

        red   = (int)value;
        green = 0;
        blue  = 0;
        if (red > 5300) {
            red = 0;
            green = 255;
            blue = 0;
        } else if (red > 255) {
            green = (int)((value-255.0)/20.0);
            if (blue > 512) {
                blue  = (int)((value-512.0)/100.0);
            }
            red   = 255;
        }

          // x stretch
        for (xs=0; xs<X_STRETCH; xs++) {
            for (ys=0; ys<Y_STRETCH; ys++) {
                setpixel(screen, xx+xs, (HEIGHT-1)-(ii*Y_STRETCH)-ys, red, green, blue);
            }
        }

        if (xx+X_STRETCH < WIDTH-X_STRETCH) {
            for (ys=0; ys<HEIGHT; ys++) {
                setpixel(screen, xx+X_STRETCH, ys, 0, 0, 255);
            }
        }

    }

    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);

    SDL_Flip(screen);
}


int main(int argc, char* argv[])
{
    SDL_Surface *screen;
    SDL_Event event;
    const SDL_VideoInfo *info;
    int keypress = 0;
    int capturing, captured_slices;

    capturing = 0;

    captured_slices    = 0;
    Audio *captured    = NULL;

    Audio *audio_input = NULL;
    printf("Spectrogram\n");

      // Initialize
    microphone_init();

    audio_input = NULL;

    size_t read_samples;

    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;

    info   = SDL_GetVideoInfo();
    if (info == NULL) {
    	printf("Unable to get video info: %s\n", SDL_GetError());
    	return 1;
    }
    WIDTH  = info->current_w;
    HEIGHT = info->current_h;

#ifdef FORCE_WIDTH
    WIDTH  = (FORCE_WIDTH);
#endif
#ifdef FORCE_HEIGHT
    HEIGHT  = (FORCE_HEIGHT);
#endif

    printf("Milliseconds per slice:    %i\n", MS_IN_TIME_SLICE);
    printf("Maximum frequency:         %.0f\n", FREQ_MULT*(HEIGHT-1));
    printf("Rate of minimum frequency: %f\n", frequency( (((double)1) * FREQ_MULT) ) );
    printf("Rate of maximum frequency: %f\n", frequency( (((double)(HEIGHT-1)) * FREQ_MULT) ) );
    printf("Samples per slice:         %ld\n", SAMPLES_IN_TIME_SLICE );
    printf("Revolutions per slice:     %.2f .. %.2f\n", (SAMPLES_IN_TIME_SLICE*frequency( (((double)1) * FREQ_MULT) ) ),  (SAMPLES_IN_TIME_SLICE*frequency( (((double)(HEIGHT-1)) * FREQ_MULT) ) ) );
    printf("Resolution:                %ix%i\n", WIDTH, HEIGHT);

    int flags = 0;

#ifndef NO_FULLSCREEN
    flags |= SDL_FULLSCREEN;
#endif
#ifndef NO_HWSUFACE
    flags |= SDL_HWSURFACE;
#endif

    if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, flags)))
    {
        SDL_Quit();
        return 1;
    }

    int xx = 0;

    while(!keypress)
    {

        audio_free( audio_input );
        audio_input = audio_make_buffer( SAMPLES_IN_TIME_SLICE, MIC_FORMAT );

          // Read in 50 ms of audio
        read_samples = microphone_read( SAMPLES_IN_TIME_SLICE, audio_input );

        audio_convert( audio_input, DOUBLE_REAL );

          // If capturing, sum the audio
        if (capturing) {
            audio_sum( audio_input, captured );
            captured_slices++;
        }

#ifdef BARTLETT_WINDOWING
        bartlett_window( audio_input );
#endif

        if (capturing)
            DrawScreen( screen, captured, xx );
        else
            DrawScreen( screen, audio_input, xx );

        xx += X_STRETCH;
        if (xx+X_STRETCH >= WIDTH) {
            xx = 0;
        }

        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
	                keypress = 1;
	                break;
                case SDL_KEYDOWN:
                    if ( event.key.keysym.sym == SDLK_SPACE ) {
                        capturing = !capturing;

                          // If we just switched to capturing, reinitialize the capturing slice
                        if (capturing) {
                            audio_free( captured );
                            captured = audio_make_buffer( SAMPLES_IN_TIME_SLICE, DOUBLE_REAL );
                            captured_slices = 0;
                        } else {
                            audio_scale( captured, 1.0 / ((Real)captured_slices) );
                        }

                    } else {
                        keypress = 1;
                        break;
                    }
            }
        }
    }

    SDL_Quit();

    return 0;
}

