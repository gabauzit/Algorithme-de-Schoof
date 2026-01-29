#ifndef TORS_RING_H
#define TORS_RING_H

#include <stdlib.h>
#include <stdarg.h>
#include <flint/flint.h>
#include <flint/fq.h>
#include <flint/fq_poly.h>
#include <flint/fmpz.h>
#include "ell_curve.h"

// Représente l'anneau quotient F_q[x,y]/(psi(x), y^2-f(x)) si y^2=f(x) définit curve
typedef struct {
    ell_curve_t curve;
    fq_poly_t psi;
} tors_ring_struct;

typedef tors_ring_struct tors_ring_t[1]; // On adopte la convention de FLINT sur les nouveaux types

// Représente la classe de A(x)+B(x)*y dans un anneau de torsion (avec deg A, deg B < deg psi)
typedef struct {
    fq_poly_t A;
    fq_poly_t B;
} tors_elem_struct;

typedef tors_elem_struct tors_elem_t[1]; // On adopte la convention de FLINT sur les nouveaux types

/*********************************/
/* PRIMITIVES ANNEAUX DE TORSION */
/*********************************/

void tors_ring_init(tors_ring_t, const fq_ctx_t);
void tors_ring_clear(tors_ring_t, const fq_ctx_t);
void tors_ring_set(tors_ring_t, const ell_curve_t, const fq_poly_t, const fq_ctx_t);

/**********************************************/
/* PRIMITIVES ELEMENTS D'UN ANNEAU DE TORSION */
/**********************************************/

void tors_elem_init(tors_elem_t, const fq_ctx_t);
void tors_elem_inits(const fq_ctx_t, tors_elem_t, ...);
void tors_elem_clear(tors_elem_t, const fq_ctx_t);
void tors_elem_clears(const fq_ctx_t, tors_elem_t, ...);
void tors_elem_set(tors_elem_t, const fq_poly_t, const fq_poly_t, const fq_ctx_t);
void tors_elem_zero(tors_elem_t, const fq_ctx_t);
void tors_elem_one(tors_elem_t, const fq_ctx_t);
void tors_elem_set_x(tors_elem_t, const fq_ctx_t);
void tors_elem_set_y(tors_elem_t, const fq_ctx_t);
void tors_elem_copy(tors_elem_t, const tors_elem_t, const fq_ctx_t);
int tors_elem_equal(const tors_elem_t, const tors_elem_t, const fq_ctx_t);
int tors_elem_is_zero(const tors_elem_t, const fq_ctx_t);
int tors_elem_is_one(const tors_elem_t, const fq_ctx_t);

/******************************************/
/* ARITHMETIQUE DANS UN ANNEAU DE TORSION */
/******************************************/

void tors_elem_neg(tors_elem_t, const tors_elem_t, const fq_ctx_t);
void tors_elem_add(tors_elem_t, const tors_elem_t, const tors_elem_t, const fq_ctx_t);
void tors_elem_sub(tors_elem_t, const tors_elem_t, const tors_elem_t, const fq_ctx_t);
void tors_elem_mul(tors_elem_t, const tors_elem_t, const tors_elem_t, const tors_ring_t, const fq_ctx_t);
void tors_elem_mul_fq(tors_elem_t, const tors_elem_t, const fq_t, const fq_ctx_t);
void tors_elem_mul_sl(tors_elem_t, const tors_elem_t, const slong, const fq_ctx_t);
void tors_elem_pow(tors_elem_t, const tors_elem_t, const fmpz_t, const tors_ring_t, const fq_ctx_t);
void tors_elem_pow_ul(tors_elem_t, const tors_elem_t, const ulong, const tors_ring_t, const fq_ctx_t);

#endif