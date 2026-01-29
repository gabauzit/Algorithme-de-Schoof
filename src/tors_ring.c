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

    // (ptr = va_arg(args, tors_ring_t *)) affecte à ptr l'adresse du prochain argument et renvoie
    // cette valeur, la boucle s'arrête donc quand ptr = NULL.
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

    // (ptr = va_arg(args, tors_ring_t *)) affecte à ptr l'adresse du prochain argument et renvoie
    // cette valeur, la boucle s'arrête donc quand ptr = NULL.
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

/**
 * Vérifie si deux éléments d'un anneau de torsion sont égaux. Renvoie 1 si c'est le cas, 0 sinon.
 */
int tors_elem_equal(const tors_elem_t op1, const tors_elem_t op2, const fq_ctx_t ctx) {
    // IL FAUDRAIT PAR FAIRE UNE REDUCTION MODULO psi DES DEUX POLYNOMES ??
    // OU ALORS IL FAUT PRESUPPOSER QUE LES DEUX POLYNOMES SONT SOUS FORME CANONIQUE
    return fq_poly_equal(op1->A, op2->A, ctx) && fq_poly_equal(op1->B, op2->B, ctx);
}

/**
 * Vérifie si un élément d'un anneau de torsion est nul. Renvoie 1 si c'est le cas, 0 sinon.
 */
int tors_elem_is_zero(const tors_elem_t op, const fq_ctx_t ctx) {
    tors_elem_t tors_zero;
    int success;

    tors_elem_init(tors_zero, ctx);
    tors_elem_zero(tors_zero, ctx);

    success = tors_elem_equal(op, tors_zero, ctx);

    tors_elem_clear(tors_zero, ctx);

    return success;
}

/**
 * Vérifie si un élément d'un anneau de torsion est l'unité. Renvoie 1 si c'est le cas, 0 sinon.
 */
int tors_elem_is_one(const tors_elem_t op, const fq_ctx_t ctx) {
    tors_elem_t tors_one;
    int success;

    tors_elem_init(tors_one, ctx);
    tors_elem_one(tors_one, ctx);

    success = tors_elem_equal(op, tors_one, ctx);

    tors_elem_clear(tors_one, ctx);

    return success;
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
 * Il faut faire très attention au cas où psi = 0, dans ce cas là il n'y a pas de réduction finale.
 * Faire un vrai produit puis la réduction n'est pas très couteux car psi_l serait de petit degré. (à voir)
 * 
 * Attention à l'aliasing, il faut créer des copies des entrées.
 */
void tors_elem_mul(tors_elem_t rop, const tors_elem_t op1, const tors_elem_t op2, const tors_ring_t tors_ring, const fq_ctx_t ctx) {
    tors_elem_t op1_copy, op2_copy;
    tors_elem_inits(ctx, op1_copy, op2_copy, NULL);
    tors_elem_copy(op1_copy, op1, ctx);
    tors_elem_copy(op2_copy, op2, ctx);
    
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
    fq_poly_mul(temp, temp, op1_copy->B, ctx);
    fq_poly_mul(temp, temp, op2_copy->B, ctx);

    fq_poly_mul(rop->A, op1_copy->A, op2_copy->A,  ctx);
    fq_poly_add(rop->A, rop->A, temp, ctx);

    // Calcul du coefficient devant y
    fq_poly_mul(rop->B, op1_copy->A, op2_copy->B, ctx);
    fq_poly_mul(temp, op1_copy->B, op2_copy->A, ctx);
    fq_poly_add(rop->B, rop->B, temp, ctx);

    if (!fq_poly_is_zero(tors_ring->psi, ctx)) {
        fq_poly_rem(rop->A, rop->A, tors_ring->psi, ctx);
        fq_poly_rem(rop->B, rop->B, tors_ring->psi, ctx);
    }

    fq_poly_clear(temp, ctx);
    fq_clear(temp_coeff, ctx);

    tors_elem_clears(ctx, op1_copy, op2_copy, NULL);
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
 * Si rop = op il y a un problème d'aliasing : op sera modifié pendant le calcul de rop !
 * On doit donc créer une nouvelle variable et y copier op.
 */
void tors_elem_pow(tors_elem_t rop, const tors_elem_t op, const fmpz_t n, const tors_ring_t tors_ring, const fq_ctx_t ctx) {
    tors_elem_t base;
    tors_elem_init(base, ctx);
    tors_elem_copy(base, op, ctx);

    tors_elem_one(rop, ctx);

    for (slong i = fmpz_bits(n) - 1; i >= 0; i--) {
        tors_elem_mul(rop, rop, rop, tors_ring, ctx);
        if (fmpz_tstbit(n, i)) tors_elem_mul(rop, rop, base, tors_ring, ctx);
    }

    tors_elem_clear(base, ctx);
}

void tors_elem_pow_ul(tors_elem_t rop, const tors_elem_t op, const ulong n, const tors_ring_t tors_ring, const fq_ctx_t ctx) {
    fmpz_t n_fmpz;
    fmpz_init_set_ui(n_fmpz, n);

    tors_elem_pow(rop, op, n_fmpz, tors_ring, ctx);

    fmpz_clear(n_fmpz);
}