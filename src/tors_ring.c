#include "tors_ring.h"

/*********************************/
/* PRIMITIVES ANNEAUX DE TORSION */
/*********************************/

void tors_ring_init(tors_ring_t tors_ring, const fq_ctx_t ctx) {
    ell_curve_init(tors_ring->curve, ctx);
    fq_poly_init(tors_ring->psi, ctx);
}

void tors_ring_clear(tors_ring_t tors_ring, const fq_ctx_t ctx) {
    ell_curve_clear(tors_ring->curve, ctx);
    fq_poly_clear(tors_ring->psi, ctx);
}

void tors_ring_set(tors_ring_t tors_ring, const ell_curve_t E, const fq_poly_t psi, const fq_ctx_t ctx) {
    ell_curve_set(tors_ring->curve, E->a, E->b, ctx);
    fq_poly_set(tors_ring->psi, psi, ctx);
}

/**********************************************/
/* PRIMITIVES ELEMENTS D'UN ANNEAU DE TORSION */
/**********************************************/

void tors_elem_init(tors_elem_t elem, const fq_ctx_t ctx) {
    fq_poly_init(elem->A, ctx);
    fq_poly_init(elem->B, ctx);
}

void tors_elem_inits(const fq_ctx_t ctx, tors_elem_t first_arg, ...) {
    va_list args;
    tors_elem_t *ptr;
    
    tors_elem_init(first_arg, ctx);

    va_start(args, first_arg);

    /* (ptr = va_arg(args, tors_ring_t *)) affecte à ptr l'adresse du prochain argument et renvoie
     * cette valeur, la boucle s'arrête donc quand ptr = NULL. */
    while ((ptr = va_arg(args, tors_elem_t *))) 
        tors_elem_init(*ptr, ctx);
    va_end(args);
}

void tors_elem_clear(tors_elem_t elem, const fq_ctx_t ctx) {
    fq_poly_clear(elem->A, ctx);
    fq_poly_clear(elem->B, ctx);
}

void tors_elem_clears(const fq_ctx_t ctx, tors_elem_t first_arg, ...) {
    va_list args;
    tors_elem_t *ptr;
    
    tors_elem_clear(first_arg, ctx);

    va_start(args, first_arg);

    /* (ptr = va_arg(args, tors_ring_t *)) affecte à ptr l'adresse du prochain argument et renvoie
     * cette valeur, la boucle s'arrête donc quand ptr = NULL. */
    while ((ptr = va_arg(args, tors_elem_t *))) 
        tors_elem_clear(*ptr, ctx);
    va_end(args);
}

void tors_elem_set(tors_elem_t rop, const fq_poly_t op_A, const fq_poly_t op_B, const fq_ctx_t ctx) {
    fq_poly_set(rop->A, op_A, ctx);
    fq_poly_set(rop->B, op_B, ctx);
}

void tors_elem_zero(tors_elem_t op, const fq_ctx_t ctx) {
    fq_poly_zero(op->A, ctx);
    fq_poly_zero(op->B, ctx);
}

void tors_elem_one(tors_elem_t op, const fq_ctx_t ctx) {
    fq_poly_one(op->A, ctx);
    fq_poly_zero(op->B, ctx);
}

/**
 * Affecte à op la valeur x.
 */
void tors_elem_set_x(tors_elem_t op, const fq_ctx_t ctx) {
    fq_t fq_one;
    fq_init(fq_one, ctx);
    fq_set_ui(fq_one, 1, ctx);

    fq_poly_zero(op->A, ctx);
    fq_poly_set_coeff(op->A, 1, fq_one, ctx);
    fq_poly_zero(op->B, ctx);

    fq_clear(fq_one, ctx);
}

/**
 * Affecte à op la valeur y.
 */
void tors_elem_set_y(tors_elem_t op, const fq_ctx_t ctx) {
    fq_poly_zero(op->A, ctx);
    fq_poly_one(op->B, ctx);
}

void tors_elem_copy(tors_elem_t target, const tors_elem_t source, const fq_ctx_t ctx) {
    tors_elem_set(target, source->A, source->B, ctx);
}

void tors_elem_swap(tors_elem_t op1, tors_elem_t op2, const fq_ctx_t ctx) {
    fq_poly_swap(op1->A, op2->A, ctx);
    fq_poly_swap(op1->B, op2->B, ctx);
}

/**
 * Vérifie si deux éléments d'un anneau de torsion sont égaux. Renvoie 1 si c'est le cas, 0 sinon.
 */
int tors_elem_equal(const tors_elem_t op1, const tors_elem_t op2, const fq_ctx_t ctx) {
    return fq_poly_equal(op1->A, op2->A, ctx) && fq_poly_equal(op1->B, op2->B, ctx);
}

/**
 * Vérifie si un élément d'un anneau de torsion est nul. Renvoie 1 si c'est le cas, 0 sinon.
 */
int tors_elem_is_zero(const tors_elem_t op, const fq_ctx_t ctx) {
    return fq_poly_is_zero(op->A, ctx) && fq_poly_is_zero(op->B, ctx);
}

/**
 * Vérifie si un élément d'un anneau de torsion est l'unité. Renvoie 1 si c'est le cas, 0 sinon.
 */
int tors_elem_is_one(const tors_elem_t op, const fq_ctx_t ctx) {
    return fq_poly_is_one(op->A, ctx) && fq_poly_is_zero(op->B, ctx);
}

/******************************************/
/* ARITHMETIQUE DANS UN ANNEAU DE TORSION */
/******************************************/

void tors_elem_neg(tors_elem_t rop, const tors_elem_t op, const fq_ctx_t ctx) {
    fq_poly_neg(rop->A, op->A, ctx);
    fq_poly_neg(rop->B, op->B, ctx);
}

void tors_elem_add(tors_elem_t rop, const tors_elem_t op1, const tors_elem_t op2, const fq_ctx_t ctx) {
    fq_poly_add(rop->A, op1->A, op2->A, ctx);
    fq_poly_add(rop->B, op1->B, op2->B, ctx);
}

void tors_elem_sub(tors_elem_t rop, const tors_elem_t op1, const tors_elem_t op2, const fq_ctx_t ctx) {
    fq_poly_sub(rop->A, op1->A, op2->A, ctx);
    fq_poly_sub(rop->B, op1->B, op2->B, ctx);
}

/**
 * c.f Proposition 4.1 du rapport.
 */
void tors_elem_mul(tors_elem_t rop, const tors_elem_t op1, const tors_elem_t op2, const tors_ring_t tors_ring, const fq_ctx_t ctx) {
    tors_elem_t res;
    tors_elem_init(res, ctx);
    
    fq_poly_t temp;
    fq_t temp_coeff;
    fq_init(temp_coeff, ctx);

    // Définition de temp = x^3 + a*x + b
    fq_poly_init2(temp, 4, ctx); // fq_poly_init2() initialise les coefficients à 0
    fq_one(temp_coeff, ctx);
    fq_poly_set_coeff(temp, 3, temp_coeff, ctx);
    fq_poly_set_coeff(temp, 1, tors_ring->curve->a, ctx);
    fq_poly_set_coeff(temp, 0, tors_ring->curve->b, ctx);

    // Calcul du coefficient constant en y
    fq_poly_mul(temp, temp, op1->B, ctx);
    fq_poly_mul(temp, temp, op2->B, ctx);

    fq_poly_mul(res->A, op1->A, op2->A,  ctx);
    fq_poly_add(res->A, res->A, temp, ctx);

    // Calcul du coefficient devant y
    fq_poly_mul(res->B, op1->A, op2->B, ctx);
    fq_poly_mul(temp, op1->B, op2->A, ctx);
    fq_poly_add(res->B, res->B, temp, ctx);

    if (!fq_poly_is_zero(tors_ring->psi, ctx)) {
        fq_poly_rem(res->A, res->A, tors_ring->psi, ctx);
        fq_poly_rem(res->B, res->B, tors_ring->psi, ctx);
    }

    tors_elem_swap(res, rop, ctx);

    fq_poly_clear(temp, ctx);
    fq_clear(temp_coeff, ctx);
    tors_elem_clear(res, ctx);
}

void tors_elem_mul_fq(tors_elem_t rop, const tors_elem_t op, const fq_t a, const fq_ctx_t ctx) {
    fq_poly_scalar_mul_fq(rop->A, op->A, a, ctx);
    fq_poly_scalar_mul_fq(rop->B, op->B, a, ctx);
}

void tors_elem_mul_sl(tors_elem_t rop, const tors_elem_t op, const slong n, const fq_ctx_t ctx) {
    fq_t n_fq;
    fq_init(n_fq, ctx);
    fq_set_si(n_fq, n, ctx);
    tors_elem_mul_fq(rop, op, n_fq, ctx);
    fq_clear(n_fq, ctx);
}

/**
 * Algorithme Square & Double en lisant les bits de l'exposant de gauche à droite.
 */
void tors_elem_pow(tors_elem_t rop, const tors_elem_t op, const fmpz_t n, const tors_ring_t tors_ring, const fq_ctx_t ctx) {
    tors_elem_t res;
    tors_elem_init(res, ctx);
    tors_elem_one(res, ctx);

    for (slong i = fmpz_bits(n) - 1; i >= 0; i--) {
        tors_elem_mul(res, res, res, tors_ring, ctx);
        if (fmpz_tstbit(n, i)) tors_elem_mul(res, res, op, tors_ring, ctx);
    }

    tors_elem_swap(res, rop, ctx);
    tors_elem_clear(res, ctx);
}

/**
 * On a préféré dupliquer le code plutôt que de faire une copie d'un ulong vers un fmpz_t et de faire appel
 * à la fonction précédente, ça serait plus lourd sachant que cette fonction est souvent appelée.
 */
void tors_elem_pow_ul(tors_elem_t rop, const tors_elem_t op, const ulong n, const tors_ring_t tors_ring, const fq_ctx_t ctx) {
    tors_elem_t res;
    tors_elem_init(res, ctx);
    tors_elem_one(res, ctx);

    for (slong i = FLINT_BIT_COUNT(n) - 1; i >= 0; i--) {
        tors_elem_mul(res, res, res, tors_ring, ctx);
        if (n & (1UL << i)) tors_elem_mul(res, res, op, tors_ring, ctx);
    }

    tors_elem_swap(res, rop, ctx);
    tors_elem_clear(res, ctx);
}