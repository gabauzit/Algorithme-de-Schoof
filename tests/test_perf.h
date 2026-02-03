#ifndef TEST_PERF_H
#define TEST_PERF_H

#ifndef NUM_TRIALS
#define NUM_TRIALS 5
#endif

#ifndef MIN_BITS
#define MIN_BITS 8 // Il faut que MIN_BITS soit supérieur à 4, sinon FLINT peut ne pas supporter
#endif

#ifndef MAX_BITS
#define MAX_BITS 64
#endif

#define TOTAL_NUM_TRIALS (NUM_TRIALS * (MAX_BITS - MIN_BITS + 1))

#include <stdio.h>
#include <flint/flint.h>
#include <flint/fmpz.h>
#include <flint/fq.h>
#include <time.h>
#include "ell_curve.h"
#include "schoof.h"

/**
 * Les tests ne sont effectués que pour q premier.
 */

int main();

#endif