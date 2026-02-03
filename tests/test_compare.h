#ifndef TEST_COMPARE_H
#define TEST_COMPARE_H

#ifndef NUM_TRIALS
#define NUM_TRIALS 5
#endif

#ifndef MIN_BITS
#define MIN_BITS 8 // Il faut que MIN_BITS soit supérieur à 4, sinon FLINT peut ne pas supporter
#endif

#ifndef MAX_BITS
#define MAX_BITS 32
#endif

#define TOTAL_NUM_TRIALS (NUM_TRIALS * (MAX_BITS - MIN_BITS + 1))

#include <stdio.h>
#include <flint/flint.h>
#include <flint/fmpz.h>
#include <flint/fq.h>
#include "ell_curve.h"
#include "schoof.h"

/**
 * Les tests ne sont effectués que pour q premier.
 */

void naive_num_of_points(fmpz_t, const fq_t, const fq_t, const fq_ctx_t);
int test_schoof(const ell_curve_t, const fq_ctx_t);
int main();

#endif