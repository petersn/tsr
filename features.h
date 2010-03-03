 // Feature extractor library header

#ifndef TSR_FEATURES_HEADER
#define TSR_FEATURES_HEADER

#define BUCKET_COUNT 1

double key_frequencies[BUCKET_COUNT];

typedef struct {

    double buckets[BUCKET_COUNT];

} Features;

  // Functions:
Features features_extract( Audio *audio, double *key_frequencies );

#endif

