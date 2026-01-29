#ifndef SCHOOF_H
#define SCHOOF_H

#include <flint/flint.h>
#include <flint/fmpz.h>
#include <flint/fq.h>
#include <flint/ulong_extras.h>
#include "tors_ring.h"
#include "ell_curve.h"
#include "ell_point.h"
#include "list.h"

#define PSI(n) list_fq_poly_get(list_psi, n)

// Comme je ne sais pas de combien de nombres premiers j'aurai besoin pour reconstituer a_q, il faut que
// je puisse ajouter dynamiquement des nombres premiers et les réduction a_q modulo ces nombres. On crée
// donc une structure de liste en cellules, comme on n'aura que peu de nombres premiers c'est pas trop
// couteux d'accéder à un élément.

void update_list_div_poly(list_fq_poly_t, const ell_curve_t, const ulong, const fq_ctx_t);
void ell_schoof(fmpz_t, const ell_curve_t, const fq_ctx_t);
int schoof(fmpz_t, const fq_t, const fq_t, const fq_ctx_t);

#endif