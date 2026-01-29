#ifndef TEST_COMPARE_H
#define TEST_COMPARE_H

#ifndef NUM_TRIALS
#define NUM_TRIALS 5
#endif

#ifndef MIN_BITS
#define MIN_BITS 8 // IL FAUT QUE CA SOIT SUPERIEUR OU EGALE A 4, SINON FLINT PEUT NE PAS SUPPORTER
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

// Est-ce qu'on pourrait aussi faire en fonction du nombre de bits de q ?
// fmpz_randprime() permet de trouver un nombre premier aléatoire avec un nombre de bits donné
// On pourrait juste traiter le cas où q est premier, vu que normalement ça compte pas.

// Il faut se rappeler que les tests se font que sur des nombres premiers.

// ATTENTION, IL PEUT Y AVOIR UN NOMBRE DE BITS Où LA BOUCLE QUI CHERCHE DES COEFFICIENTS EST INFINIE
// IL FAUT DEJA AU MOINS 3 BITS POUR EVITER 2 ET 3

void naive_num_of_points(fmpz_t, const fq_t, const fq_t, const fq_ctx_t);
int test_schoof(const ell_curve_t, const fq_ctx_t);
int main();

#endif