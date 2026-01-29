#ifndef ELL_CURVE_H
#define ELL_CURVE_H

#include <stdlib.h>
#include <flint/flint.h>
#include <flint/fq.h>
#include <flint/fmpz.h>

// Représente la courbe elliptique sous forme de Weierstrass y^2 = x^3 + a*x + b
typedef struct {
    fq_t a;
    fq_t b;
} ell_curve_struct;

typedef ell_curve_struct ell_curve_t[1]; // On adopte la convention de FLINT sur les nouveaux types

void ell_curve_init(ell_curve_t, const fq_ctx_t);
void ell_curve_clear(ell_curve_t, const fq_ctx_t);
int ell_curve_set(ell_curve_t, const fq_t, const fq_t, const fq_ctx_t);
int ell_curve_init_set(ell_curve_t, const fq_t, const fq_t, const fq_ctx_t);

#endif