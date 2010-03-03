 // Production model header

#ifndef TSR_PRODUCTION_HEADER
#define TSR_PRODUCTION_HEADER

#define CANDIDATES 12

#include "fourier.h"

typedef struct {

    char *name;
    Fourier vector;

} Phoneme;

typedef struct {

    double score;

    Phoneme **phons;

} Candidate;

Candidate candidates[CANDIDATES];

  // Functions:
void init_candidates();
void sort_candidates();

#endif

