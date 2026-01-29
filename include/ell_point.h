#ifndef ELL_POINT_H
#define ELL_POINT_H

#include <flint/flint.h>
#include <flint/fq.h>
#include "ell_curve.h"
#include "tors_ring.h"

/**
 * Les courbes elliptiques sont définies sur F_q, mais ici les opérations de groupe sont faites sur
 * des anneaux de torsion. C’est assez inhabituel, mais complètement suffisant pour ce dont on a besoin.
 * Dans un autre contexte, ce qu'on fait ici serait clairement problématique : on ne pourrait même pas
 * définir correctement la loi de groupe sur le corps de base.
 */

/**
 * On utilise des coordonnées jacobiennes pour éviter les divisions puisqu'on travaille à coefficients
 * dans un anneau. L'avantage est que ça rend la loi de groupe entièrement polynomiale. Un point (X, Y, Z)
 * en coordonnées projectives représente le point affine (X/Z^2, Y/Z^3) si Z est non nul, si Z = 0
 * c’est le point à l’infini (1, 1, 0).
 */

typedef struct {
    tors_elem_t X;
    tors_elem_t Y;
    tors_elem_t Z;
} ell_point_struct;

typedef ell_point_struct ell_point_t[1]; // On adopte la convention de FLINT sur les nouveaux types

/**************/
/* PRIMITIVES */
/**************/

void ell_point_init(ell_point_t, const fq_ctx_t);
void ell_point_clear(ell_point_t, const fq_ctx_t);
void ell_point_set_infinity(ell_point_t, const fq_ctx_t);
void ell_point_copy(ell_point_t, const ell_point_t, const fq_ctx_t);
int ell_point_equal(const ell_point_t, const ell_point_t, const tors_ring_t R, const fq_ctx_t);
int ell_point_is_infinity(const ell_point_t, const fq_ctx_t);

/******************************************/
/* OPERATIONS SUR LES COURBES ELLIPTIQUES */
/******************************************/

void ell_point_neg(ell_point_t, const ell_point_t, const fq_ctx_t);
void ell_point_double(ell_point_t, const ell_point_t, const tors_ring_t, const fq_ctx_t);
void ell_point_add(ell_point_t, const ell_point_t, const ell_point_t, const tors_ring_t, const fq_ctx_t);
void ell_point_mul(ell_point_t, const ell_point_t, const fmpz_t, const tors_ring_t, const fq_ctx_t);

#endif