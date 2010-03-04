 // Audio types header

#ifndef TSR_AUDIO_HEADER
#define TSR_AUDIO_HEADER

#ifdef  USE_DOUBLE
typedef double Real;

#ifdef USE_FLOAT
#warning You should not give both -DUSE_FLOAT and -DUSE_DOUBLE (assuming double)
#endif

#else

#ifndef  USE_FLOAT
#warning You should give either -DUSE_FLOAT or -DUSE_DOUBLE to select the internal audio representation (assuming float)
#endif

typedef float Real;

#endif

  // Suggested variable names: format (aformat), bformat, cformat
typedef enum {
    NO_FORMAT,
    EIGHT_BIT_UNSIGNED,
    SIXTEEN_BIT_SIGNED,
    DOUBLE_REAL,

    MAX_AUDIO_FORMAT
} Audio_Format;

const char *format_names[MAX_AUDIO_FORMAT];
size_t sample_size[MAX_AUDIO_FORMAT];

  // Suggested variable names: audio, budio, cudio
typedef struct {
    Audio_Format format;
    size_t samples;
    void *data;
} Audio;

  // Standard audio functions:
void audio_free( Audio *audio );
Audio *audio_duplicate( Audio *audio );
Audio *audio_make_buffer( size_t samples, Audio_Format format );

void audio_convert( Audio *audio, Audio_Format format );
void audio_sum( Audio *audio, Audio *budio );
void audio_scale( Audio *audio, Real scaling );

#endif

