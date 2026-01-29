#include "ell_point.h"

/**************/
/* PRIMITIVES */
/**************/

void ell_point_init(ell_point_t P, const fq_ctx_t ctx) {
    tors_elem_inits(ctx, P->X, P->Y, P->Z, NULL);
}

void ell_point_clear(ell_point_t P, const fq_ctx_t ctx) {
    tors_elem_clears(ctx, P->X, P->Y, P->Z, NULL);
}

void ell_point_set_infinity(ell_point_t P, const fq_ctx_t ctx) {
    tors_elem_one(P->X, ctx);
    tors_elem_one(P->Y, ctx);
    tors_elem_zero(P->Z, ctx);
}

void ell_point_copy(ell_point_t rop, const ell_point_t op, const fq_ctx_t ctx) {
    tors_elem_copy(rop->X, op->X, ctx);
    tors_elem_copy(rop->Y, op->Y, ctx);
    tors_elem_copy(rop->Z, op->Z, ctx);
}

/**
 * Vérifie si deux points d'une courbe elliptique sont égaux. Renvoie 1 si c'est la cas, 0 sinon.
 */
int ell_point_equal(const ell_point_t op1, const ell_point_t op2, const tors_ring_t tors_ring, const fq_ctx_t ctx) {
    if (ell_point_is_infinity(op1, ctx)) {
        return ell_point_is_infinity(op2, ctx);
    }
    
    if (ell_point_is_infinity(op2, ctx)) return 0;

    tors_elem_t temp1, temp2;
    int success = 1;

    tors_elem_init(temp1, ctx);
    tors_elem_pow_ul(temp1, op2->Z, 2, tors_ring, ctx);
    tors_elem_mul(temp1, temp1, op1->X, tors_ring, ctx);

    tors_elem_init(temp2, ctx);
    tors_elem_pow_ul(temp2, op1->Z, 2, tors_ring, ctx);
    tors_elem_mul(temp2, temp2, op2->X, tors_ring, ctx);

    success = tors_elem_equal(temp1, temp2, ctx);

    if (success) {
        tors_elem_pow_ul(temp1, op2->Z, 3, tors_ring, ctx);
        tors_elem_mul(temp1, temp1, op1->Y, tors_ring, ctx);

        tors_elem_pow_ul(temp2, op1->Z, 3, tors_ring, ctx);
        tors_elem_mul(temp2, temp2, op2->Y, tors_ring, ctx);

        success = tors_elem_equal(temp1, temp2, ctx);
    }

    tors_elem_clear(temp1, ctx);
    tors_elem_clear(temp2, ctx);

    return success;
}

/**
 * Vérifie si un point d'une courbe elliptique est le point à l'infini. Renvoie 1 si c'est le cas, 0 sinon.
 */
int ell_point_is_infinity(const ell_point_t op, const fq_ctx_t ctx) {
    return tors_elem_is_zero(op->Z, ctx);
}

/******************************************/
/* OPERATIONS SUR LES COURBES ELLIPTIQUES */
/******************************************/

void ell_point_neg(ell_point_t rop, const ell_point_t op, const fq_ctx_t ctx) {
    ell_point_copy(rop, op, ctx);
    tors_elem_neg(rop->Y, rop->Y, ctx);
}

void ell_point_double(ell_point_t rop, const ell_point_t op, const tors_ring_t tors_ring, const fq_ctx_t ctx) {
    if (ell_point_is_infinity(op, ctx) || tors_elem_is_zero(op->Y, ctx)) { // On teste si op est d'ordre 1 ou 2
        ell_point_set_infinity(rop, ctx);
        return;
    }

    ell_point_t op_copy;
    ell_point_init(op_copy, ctx);
    ell_point_copy(op_copy, op, ctx);
    
    tors_elem_t A, B, C, D, temp;
    tors_elem_inits(ctx, A, B, C, D, temp, NULL);

    // Calcul de A
    tors_elem_pow_ul(A, op_copy->Y, 2, tors_ring, ctx);

    // Calcul de B
    tors_elem_mul(B, op_copy->X, A, tors_ring, ctx);
    tors_elem_mul_sl(B, B, 4, ctx);

    // Calcul de C
    tors_elem_pow_ul(C, A, 2, tors_ring, ctx);
    tors_elem_mul_sl(C, C, 8, ctx);

    // Calcul de D
    tors_elem_pow_ul(D, op_copy->X, 2, tors_ring, ctx);
    tors_elem_mul_sl(D, D, 3, ctx);

    tors_elem_pow_ul(temp, op_copy->Z, 4, tors_ring, ctx);
    tors_elem_mul_fq(temp, temp, tors_ring->curve->a, ctx);

    tors_elem_add(D, D, temp, ctx);

    // Calcul de rop->X
    tors_elem_pow_ul(rop->X, D, 2, tors_ring, ctx);

    tors_elem_mul_sl(temp, B, 2, ctx);

    tors_elem_sub(rop->X, rop->X, temp, ctx);

    // Calcul de rop->Y
    tors_elem_sub(rop->Y, B, rop->X, ctx);
    tors_elem_mul(rop->Y, D, rop->Y, tors_ring, ctx);
    tors_elem_sub(rop->Y, rop->Y, C, ctx);

    // Calcul de rop->Z
    tors_elem_mul(rop->Z, op_copy->Y, op_copy->Z, tors_ring, ctx);
    tors_elem_mul_sl(rop->Z, rop->Z, 2, ctx);

    tors_elem_clears(ctx, A, B, C, D, temp, NULL);
    ell_point_clear(op_copy, ctx);
}

/** 
 * Comme on est sur un anneau, être affine n'est pas équivalent à Z = 1, mais en pratique on définit des
 * points affines avec Z = 1 par convention, et un des arguments sera souvent (voire tout le temps) dans
 * ce cas là. D'ailleurs l'astuce mixte est utile seulement si Z = 1, et pas seulement avec Z inversible.
*/
void ell_point_add(ell_point_t rop, const ell_point_t op1, const ell_point_t op2, const tors_ring_t tors_ring, const fq_ctx_t ctx) {
    // On teste si op1 ou op2 est le point à l'infini
    if (ell_point_is_infinity(op1, ctx)) {
        ell_point_copy(rop, op2, ctx);
        return;
    }
    if (ell_point_is_infinity(op2, ctx)) {
        ell_point_copy(rop, op1, ctx);
        return;
    }
   
    // On teste si op1 = op2
    if (ell_point_equal(op1, op2, tors_ring, ctx)) {
        ell_point_double(rop, op1, tors_ring, ctx);
        return;
    }

    // On teste si op1 = -op2
    ell_point_t op2_neg;
    ell_point_init(op2_neg, ctx);
    ell_point_neg(op2_neg, op2, ctx);

    if (ell_point_equal(op1, op2_neg, tors_ring, ctx)) {
        ell_point_set_infinity(rop, ctx);
        ell_point_clear(op2_neg, ctx);
        return;
    }
    ell_point_clear(op2_neg, ctx);

    ell_point_t op1_copy, op2_copy;
    ell_point_init(op1_copy, ctx);
    ell_point_init(op2_copy, ctx);
    ell_point_copy(op1_copy, op1, ctx);
    ell_point_copy(op2_copy, op2, ctx);

    ell_point_struct *op_aff = NULL, *op_jac = NULL;
    if (tors_elem_is_one(op1->Z, ctx)) {
        op_aff = op1_copy;
        op_jac = op2_copy;        
    } else if (tors_elem_is_one(op2->Z, ctx)) {
        op_aff = op2_copy;
        op_jac = op1_copy;
    }

    if (op_aff == NULL) {
        // Il n'y a a priori pas de point affine, on utilise la formule générale (ça n'arrivera jamais en pratique)
        tors_elem_t A, B, C, D, E, F, G, H, I, J, K, L, M, N, temp;
        tors_elem_inits(ctx, A, B, C, D, E, F, G, H, I, J, K, L, M, N, temp, NULL);

        // Calculs intermédiaires
        tors_elem_pow_ul(A, op1_copy->Z, 2, tors_ring, ctx);
        tors_elem_pow_ul(B, op2_copy->Z, 2, tors_ring, ctx);
        tors_elem_mul(C, op1_copy->X, B, tors_ring, ctx);
        tors_elem_mul(D, op2_copy->X, A, tors_ring, ctx);
        tors_elem_mul(E, op1_copy->Z, op2_copy->Z, tors_ring, ctx);
        tors_elem_mul(F, op1_copy->Z, E, tors_ring, ctx);
        tors_elem_mul(G, op2_copy->Z, E, tors_ring, ctx);
        tors_elem_mul(H, op1_copy->Y, G, tors_ring, ctx);
        tors_elem_mul(I, op2_copy->Y, F, tors_ring, ctx);
        tors_elem_sub(J, D, C, ctx);
        tors_elem_sub(K, I, H, ctx);
        tors_elem_pow_ul(L, J, 2, tors_ring, ctx);
        tors_elem_mul(M, J, L, tors_ring, ctx);
        tors_elem_mul(N, C, L, tors_ring, ctx);

        // Calcul de rop->X
        tors_elem_pow_ul(rop->X, K, 2, tors_ring, ctx);
        tors_elem_sub(rop->X, rop->X, M, ctx);
        tors_elem_mul_sl(temp, N, 2, ctx);
        tors_elem_sub(rop->X, rop->X, temp, ctx);

        // Calcul de rop->Y
        tors_elem_sub(rop->Y, N, rop->X, ctx);
        tors_elem_mul(rop->Y, K, rop->Y, tors_ring, ctx);
        tors_elem_mul(temp, H, M, tors_ring, ctx);
        tors_elem_sub(rop->Y, rop->Y, temp, ctx);

        // Calcul de rop->Z
        tors_elem_mul(rop->Z, J, E, tors_ring, ctx);

        tors_elem_clears(ctx, A, B, C, D, E, F, G, H, I, J, K, L, M, N, temp, NULL);
    } else {
        // Il y a un point affine, on utilise alors l'addition mixte
        tors_elem_t A, B, C, D, E, F, G, H, I, temp;
        tors_elem_inits(ctx, A, B, C, D, E, F, G, H, I, temp, NULL);

        // Calculs intermédiaires
        tors_elem_pow_ul(A, op_jac->Z, 2, tors_ring, ctx);
        tors_elem_mul(B, op_jac->Z, A, tors_ring, ctx);
        tors_elem_mul(C, op_aff->X, A, tors_ring, ctx);
        tors_elem_mul(D, op_aff->Y, B, tors_ring, ctx);
        tors_elem_sub(E, C, op_jac->X, ctx);
        tors_elem_sub(F, D, op_jac->Y, ctx);
        tors_elem_pow_ul(G, E, 2, tors_ring, ctx);
        tors_elem_mul(H, G, E, tors_ring, ctx);
        tors_elem_mul(I, op_jac->X, G, tors_ring, ctx);

        // Calcul de rop->X
        tors_elem_pow_ul(rop->X, F, 2, tors_ring, ctx);

        tors_elem_mul_sl(temp, I, 2, ctx);
        tors_elem_add(temp, H, temp, ctx);

        tors_elem_sub(rop->X, rop->X, temp, ctx);

        // Calcul de rop->Y
        tors_elem_sub(rop->Y, I, rop->X, ctx);
        tors_elem_mul(rop->Y, F, rop->Y, tors_ring, ctx);

        tors_elem_mul(temp, op_jac->Y, H, tors_ring, ctx);

        tors_elem_sub(rop->Y, rop->Y, temp, ctx);

        // Calcul de rop->Z
        tors_elem_mul(rop->Z, op_jac->Z, E, tors_ring, ctx);

        // Libération de la mémoire
        tors_elem_clears(ctx, temp, A, B, C, D, E, F, G, H, I, NULL);
    }

    ell_point_clear(op1_copy, ctx);
    ell_point_clear(op2_copy, ctx);
}

/**
 * Calcule les itérés d'un point d'une courbe elliptique à coefficients dans un anneau de torsion
 * 
 * Si rop = op il y a un problème d'aliasing : op sera modifié pendant le calcul de rop !
 * On doit donc créer une nouvelle variable et y copier op.
 */
void ell_point_mul(ell_point_t rop, const ell_point_t op, const fmpz_t n, const tors_ring_t tors_ring, const fq_ctx_t ctx) {
    if (fmpz_sgn(n) < 0) {
        ell_point_t op_neg;
        fmpz_t n_neg;

        fmpz_init(n_neg);
        fmpz_neg(n_neg, n);
        
        ell_point_init(op_neg, ctx);
        ell_point_neg(op_neg, op, ctx);
        ell_point_mul(rop, op_neg, n_neg, tors_ring, ctx);

        ell_point_clear(op_neg, ctx);
        fmpz_clear(n_neg);
        return;
    }

    ell_point_t op_copy;
    ell_point_init(op_copy, ctx);
    ell_point_copy(op_copy, op, ctx);

    ell_point_set_infinity(rop, ctx);
    
    for (slong i = fmpz_bits(n) - 1; i >= 0; i--) {
        // i est de type slong car s'il était de type ulong, dès qu'il attendrait la valeur 0 la commande
        // i-- attribuerait à i la valeur maximale pour le type ulong, et on aurait une boucle infini !
        ell_point_double(rop, rop, tors_ring, ctx);
        if (fmpz_tstbit(n, i)) ell_point_add(rop, rop, op_copy, tors_ring, ctx);
    }

    ell_point_clear(op_copy, ctx);
}