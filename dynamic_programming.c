 // Dynamic programming implementation for comparing feature vector-vectors
#include <stdlib.h>

#include "dynamic_programming.h"

inline Real vector_compare( Features *featuresA, Features *featuresB ) {
    Real accumulate = 0.0;
    int ii;

    for (ii=0; ii<BUCKET_COUNT; ii++) {
        accumulate += featuresA->buckets[ii] * featuresB->buckets[ii];
    }

    return accumulate;
}

Real dp_compare( int countA, Features *featuresA, int countB, Features *featuresB ) {
    Real **matrix;
    Real *temp;
    int xx, yy;

      // Allocate space for each row pointer
    matrix = malloc( sizeof( Real * ) * countA );

      // Allocate space for each row
    for (yy=0; yy<countA; yy++) {
        temp = malloc( sizeof( Real ) * countB );
        for (xx=0; xx<countB; xx++) {
            temp[xx] = 0.0;
        }
        matrix[yy] = temp;
    }

    Real minimum;
    Real value = 0.0;

      // Begin propagating down the matrix
    for (yy=0; yy<countA; yy++) {
          // Propagate across the matrix
        for (xx=0; xx<countB; xx++) {
            minimum = INFINITY;
            if (xx > 0 && (value = matrix[yy][xx-1] + vector_compare( &(featuresA[yy]), &(featuresB[xx-1]) ) )< minimum) minimum = value;
            if (yy > 0 && (value = matrix[yy-1][xx] + vector_compare( &(featuresA[yy-1]), &(featuresB[xx]) ) )< minimum) minimum = value;
            if (minimum < INFINITY) matrix[yy][xx] = value;
        }
    }

      // Finally, free the data structure we built
    for (yy=0; yy<countA; yy++) {
          // Free each row
        free( matrix[yy] );
    }

      // Free all the columns
    free( matrix );

    return 0.0;

}

