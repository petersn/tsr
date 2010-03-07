 // Dynamic programming header

#ifndef TSR_DYNAMIC_HEADER
#define TSR_DYNAMIC_HEADER

#include "features.h"

  // In case the underlying float system doesn't support "infinity", let's define a biiiiiiig number as inf
#define INFINITY 1000000000000

Real dp_compare( int countA, Features *featuresA, int countB, Features *featuresB );

#endif

