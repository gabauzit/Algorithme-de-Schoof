#include "ell_curve.h"

void ell_curve_init(ell_curve_t E, const fq_ctx_t ctx) {
    fq_init(E->a, ctx);
    fq_init(E->b, ctx);
}

void ell_curve_clear(ell_curve_t E, const fq_ctx_t ctx) {
    fq_clear(E->a, ctx);
    fq_clear(E->b, ctx);
}

/**
 * Définit les paramètres d'une courbe elliptique.
 * 
 * Si la courbe obtenue est bien elliptique, lui attribue les nouveaux paramètres et renvoie EXIT_SUCCESS,
 * sinon laisse l'entrée inchangée est renvoie EXIT_FAILURE.
 */
int ell_curve_set(ell_curve_t E, const fq_t a, const fq_t b, const fq_ctx_t ctx) {
    // On vérifie que le discriminant 4*a^3 + 27*b^2 est non-nul
    fq_t disc, temp;
    fq_init(disc, ctx);
    fq_init(temp, ctx);

    fq_pow_ui(temp, a, 3, ctx);
    fq_mul_ui(temp, temp, 4, ctx);
    fq_pow_ui(disc, b, 2, ctx);
    fq_mul_ui(disc, disc, 27, ctx);
    fq_add(disc, disc, temp, ctx);

    int success = EXIT_FAILURE;

    if (!fq_is_zero(disc, ctx)) {
        fq_set(E->a, a, ctx);
        fq_set(E->b, b, ctx);
        success = EXIT_SUCCESS;
    }

    fq_clear(disc, ctx);
    fq_clear(temp, ctx);

    return success;
}