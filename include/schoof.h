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

/**
 * Section 5.5 du rapport
 */

#define PSI(n) list_fq_poly_get(list_psi, n)

void update_list_div_poly(list_fq_poly_t, const ell_curve_t, const ulong, const fq_ctx_t);
void ell_schoof(fmpz_t, const ell_curve_t, const fq_ctx_t);
int schoof(fmpz_t, const fq_t, const fq_t, const fq_ctx_t);

#endif