 // Feature extractor library header

#ifndef TSR_FEATURES_HEADER
#define TSR_FEATURES_HEADER

#ifndef BUCKET_COUNT
#define BUCKET_COUNT 32
#endif

#ifndef SAMPLING_MIN_FREQ
#define SAMPLING_MIN_FREQ 400
#endif

#ifndef SAMPLING_MAX_FREQ
#define SAMPLING_MAX_FREQ 2500
#endif

  // Derive the affine function: freq(ii) = add + ii * mult
#define AFFINE_SAMPLING_ADD  SAMPLING_MIN_FREQ
#define AFFINE_SAMPLING_MULT ((((Real)SAMPLING_MAX_FREQ)-((Real)SAMPLING_MIN_FREQ)) / ((Real)BUCKET_COUNT) )

//double key_frequencies[BUCKET_COUNT];
Real key_frequency(int index);

typedef struct {
    Real buckets[BUCKET_COUNT];
} Features;

  // Functions:
Features features_extract( Audio *audio );
void features_pretty( Features features );
void zero_features( Features *features );
void scale_features( Features *features, Real scalar );
void copy_scaled_features( Features *features, Features *beatures, Real scalar );
void add_features( Features *features, Features *beatures );
void subtract_features( Features *features, Features *beatures );

#endif

