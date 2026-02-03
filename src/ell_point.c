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

void ell_point_swap(ell_point_t op1, ell_point_t op2, const fq_ctx_t ctx) {
    tors_elem_swap(op1->X, op2->X, ctx);
    tors_elem_swap(op1->Y, op2->Y, ctx);
    tors_elem_swap(op1->Z, op2->Z, ctx);
}

/**
 * Vérifie si un point d'une courbe elliptique est le point à l'infini. Renvoie 1 si c'est le cas, 0 sinon.
 */
int ell_point_is_infinity(const ell_point_t op, const fq_ctx_t ctx) {
    return tors_elem_is_zero(op->Z, ctx);
}

/**
 * Vérifie si deux points d'une courbe elliptique sont égaux. Renvoie 1 si c'est la cas, 0 sinon.
 * c.f Proposition 4.9 du rapport.
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

/******************************************/
/* OPERATIONS SUR LES COURBES ELLIPTIQUES */
/******************************************/

void ell_point_neg(ell_point_t rop, const ell_point_t op, const fq_ctx_t ctx) {
    ell_point_copy(rop, op, ctx);
    tors_elem_neg(rop->Y, rop->Y, ctx);
}

/**
 * c.f Proposition 4.6 du rapport, on a réduit le nombre de variables temporaires du mieux possible.
 */
void ell_point_double(ell_point_t rop, const ell_point_t op, const tors_ring_t tors_ring, const fq_ctx_t ctx) {
    // On teste si op est d'ordre 1 ou 2
    if (ell_point_is_infinity(op, ctx) || tors_elem_is_zero(op->Y, ctx)) {
        ell_point_set_infinity(rop, ctx);
        return;
    }

    ell_point_t res;
    ell_point_init(res, ctx);
    
    tors_elem_t A, B, C, D;
    tors_elem_inits(ctx, A, B, C, D, NULL);

    // A = Y^2
    tors_elem_pow_ul(A, op->Y, 2, tors_ring, ctx);

    // B = 4*X*A
    tors_elem_mul(B, op->X, A, tors_ring, ctx);
    tors_elem_mul_sl(B, B, 4, ctx);

    // C = 3*X^2 + a*Z^4
    tors_elem_pow_ul(D, op->Z, 4, tors_ring, ctx);
    tors_elem_mul_fq(D, D, tors_ring->curve->a, ctx);

    tors_elem_pow_ul(C, op->X, 2, tors_ring, ctx);
    tors_elem_mul_sl(C, C, 3, ctx);

    tors_elem_add(C, D, C, ctx);

    // X_3 = C^2 - 2*B
    tors_elem_pow_ul(res->X, C, 2, tors_ring, ctx);  
    tors_elem_mul_sl(D, B, 2, ctx);
    tors_elem_sub(res->X, res->X, D, ctx);

    // D = 8*A^2
    tors_elem_pow_ul(D, A, 2, tors_ring, ctx);
    tors_elem_mul_sl(D, D, 8, ctx);

    // Y_3 = C*(B - X_3) - D
    tors_elem_sub(res->Y, B, res->X, ctx);
    tors_elem_mul(res->Y, C, res->Y, tors_ring, ctx);
    tors_elem_sub(res->Y, res->Y, D, ctx);

    // Z_3 = 2*Y*Z
    tors_elem_mul(res->Z, op->Y, op->Z, tors_ring, ctx);
    tors_elem_mul_sl(res->Z, res->Z, 2, ctx);

    ell_point_swap(rop, res, ctx);

    tors_elem_clears(ctx, A, B, C, D, NULL);
    ell_point_clear(res, ctx);
}

/**
 * c.f Proposition 4.6 du rapport, on a réduit le nombre de variables temporaires du mieux possible.
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
    ell_point_t res; // res vaut -op2, et sera éventuellement réutilisée plus tard
    ell_point_init(res, ctx);
    ell_point_neg(res, op2, ctx);

    if (ell_point_equal(op1, res, tors_ring, ctx)) {
        ell_point_set_infinity(rop, ctx);
        ell_point_clear(res, ctx);
        return;
    }

    const ell_point_struct *op_aff = NULL, *op_jac = NULL;
    if (tors_elem_is_one(op1->Z, ctx)) {
        op_aff = op1;
        op_jac = op2;        
    } else if (tors_elem_is_one(op2->Z, ctx)) {
        op_aff = op2;
        op_jac = op1;
    }

    if (op_aff == NULL) {
        // Il n'y a a priori pas de point affine, on utilise la formule générale (ça n'arrivera jamais en pratique)
        tors_elem_t A, B, C, D, E, F, G, H, I, J;
        tors_elem_inits(ctx, A, B, C, D, E, F, G, H, I, J, NULL);

        // A = Z_1^2
        tors_elem_pow_ul(A, op1->Z, 2, tors_ring, ctx);

        // B = Z_2^2
        tors_elem_pow_ul(B, op2->Z, 2, tors_ring, ctx);

        // C = X_1*B
        tors_elem_mul(C, op1->X, B, tors_ring, ctx);

        // D = X_2*A
        tors_elem_mul(D, op2->X, A, tors_ring, ctx);

        // E = Z_1*Z_2
        tors_elem_mul(E, op1->Z, op2->Z, tors_ring, ctx);

        // F = Z_1*E
        tors_elem_mul(F, op1->Z, E, tors_ring, ctx);

        // G = Z_2*E
        tors_elem_mul(G, op2->Z, E, tors_ring, ctx);

        // H = Y_1*G
        tors_elem_mul(H, op1->Y, G, tors_ring, ctx);

        // I = Y_2*F
        tors_elem_mul(I, op2->Y, F, tors_ring, ctx);

        // A =  D - C
        tors_elem_sub(A, D, C, ctx);

        // B = I - H
        tors_elem_sub(B, I, H, ctx);

        // D = A^2
        tors_elem_pow_ul(D, A, 2, tors_ring, ctx);

        // E = A*D
        tors_elem_mul(E, A, D, tors_ring, ctx);

        // F = C*D
        tors_elem_mul(F, C, D, tors_ring, ctx);

        // G = 2*F
        tors_elem_mul_sl(G, F, 2, ctx);

        // X_3 = B^2 - E - G
        tors_elem_pow_ul(res->X, B, 2, tors_ring, ctx);
        tors_elem_sub(res->X, res->X, E, ctx);
        tors_elem_sub(res->X, res->X, G, ctx);

        // G = H - E
        tors_elem_mul(G, H, E, tors_ring, ctx);

        // Y_3 = B*(F - X_3) - G
        tors_elem_sub(res->Y, F, res->X, ctx);
        tors_elem_mul(res->Y, B, res->Y, tors_ring, ctx);
        tors_elem_sub(res->Y, res->Y, G, ctx);

        // Z_3 = A*E
        tors_elem_mul(res->Z, A, E, tors_ring, ctx);

        ell_point_swap(rop, res, ctx);

        tors_elem_clears(ctx, A, B, C, D, E, F, G, H, I, J, NULL);

    } else {
        // Il y a un point affine, on utilise alors l'addition mixte (c.f Proposition ???)
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

        // Calcul de X_3
        tors_elem_pow_ul(res->X, F, 2, tors_ring, ctx);

        tors_elem_mul_sl(temp, I, 2, ctx);
        tors_elem_add(temp, H, temp, ctx);

        tors_elem_sub(res->X, res->X, temp, ctx);

        // Calcul de Y_3
        tors_elem_sub(res->Y, I, res->X, ctx);
        tors_elem_mul(res->Y, F, res->Y, tors_ring, ctx);

        tors_elem_mul(temp, op_jac->Y, H, tors_ring, ctx);

        tors_elem_sub(res->Y, res->Y, temp, ctx);

        // Calcul de Z_3
        tors_elem_mul(res->Z, op_jac->Z, E, tors_ring, ctx);

        ell_point_swap(res, rop, ctx);

        // Libération de la mémoire
        tors_elem_clears(ctx, temp, A, B, C, D, E, F, G, H, I, NULL);
    }

    ell_point_clear(res, ctx);
}

/**
 * Additions itérées d'un point d'une courbe elliptique via Double & Add en lisant les bits de gauche à droite.
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

    ell_point_t res;
    ell_point_init(res, ctx);
    ell_point_set_infinity(res, ctx);
    
    for (slong i = fmpz_bits(n) - 1; i >= 0; i--) {
        // i est de type slong car s'il était de type ulong, dès qu'il attendrait la valeur 0 la commande
        // i-- attribuerait à i la valeur maximale pour le type ulong, et on aurait une boucle infini !
        ell_point_double(res, res, tors_ring, ctx);
        if (fmpz_tstbit(n, i)) ell_point_add(res, res, op, tors_ring, ctx);
    }

    ell_point_swap(res, rop, ctx);
    ell_point_clear(res, ctx);
}