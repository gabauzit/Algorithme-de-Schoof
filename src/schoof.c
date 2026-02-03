#include "schoof.h"

/**
 * Actualise list_psi en calculant tous les ψ_m encore non-calculés jusqu'à ψ_n.
 * En vérité, list_psi calcule la suite des f_n, qui coïncide avec ψ_n pour n impair.
 * c.f Proposition 3.6 du rapport.
 */
void update_list_div_poly(list_fq_poly_t list_psi, const ell_curve_t E, const ulong n, const fq_ctx_t ctx) {
    slong list_psi_len = list_fq_poly_len(list_psi);
    
    fq_t temp;
    fq_init(temp, ctx);

    fq_poly_t temp_poly;
    fq_poly_init(temp_poly, ctx);

    // ψ_0 = 0
    if (n == 0 || (n > 0 && list_psi_len == 0)) { // Si n = 0 ou (n > 0 et ψ_0 n'a pas encore été calculé)
        fq_poly_zero(temp_poly, ctx);
        list_fq_poly_add(list_psi, temp_poly, ctx);
    }

    // ψ_1 = 1
    if (n == 1 || (n > 1 && list_psi_len <= 1)) { // Si n = 1 ou (n > 1 et ψ_1 n'a pas encore été calculé)
        fq_poly_one(temp_poly, ctx);
        list_fq_poly_add(list_psi, temp_poly, ctx);
    }

    // ψ_2 = 2*y
    if (n == 2 || (n > 2 && list_psi_len <= 2)) { // Si n = 2 ou (n > 2 et ψ_2 n'a pas encore été calculé)
        fq_poly_zero(temp_poly, ctx);
        fq_set_ui(temp, 2, ctx);
        fq_poly_set_fq(temp_poly, temp, ctx);
        list_fq_poly_add(list_psi, temp_poly, ctx);
    }

    // ψ_3 = 3*x^4 + 6*a*x^2 + 12*b*x - a^2
    if (n == 3 || (n > 3 && list_psi_len <= 3)) { // Si n = 3 ou (n > 3 et ψ_3 n'a pas encore été calculé)
        fq_poly_zero(temp_poly, ctx);
        
        fq_set_ui(temp, 3, ctx);
        fq_poly_set_coeff(temp_poly, 4, temp, ctx);

        fq_mul_ui(temp, E->a, 6, ctx);
        fq_poly_set_coeff(temp_poly, 2, temp, ctx);

        fq_mul_ui(temp, E->b, 12, ctx);
        fq_poly_set_coeff(temp_poly, 1, temp, ctx);

        fq_pow_ui(temp, E->a, 2, ctx);
        fq_neg(temp, temp, ctx);
        fq_poly_set_coeff(temp_poly, 0, temp, ctx);

        list_fq_poly_add(list_psi, temp_poly, ctx);
    }

    // ψ_4 = 4*y*(x^6 + 5*a*x^4 + 20*b*x^3 - 5*a^2*x^2 - 4*a*b*x - 8*b^2 - a^3)
    if (n == 4 || (n > 4 && list_psi_len <= 4)) { // Si n = 4 ou (n > 4 et ψ_4 n'a pas encore été calculé)
        fq_t temp2;
        fq_init(temp2, ctx);
        fq_poly_zero(temp_poly, ctx);

        fq_set_ui(temp, 1, ctx);
        fq_poly_set_coeff(temp_poly, 6, temp, ctx);

        fq_mul_ui(temp, E->a, 5, ctx);
        fq_poly_set_coeff(temp_poly, 4, temp, ctx);

        fq_mul_ui(temp, E->b, 20, ctx);
        fq_poly_set_coeff(temp_poly, 3, temp, ctx);

        fq_pow_ui(temp, E->a, 2, ctx);
        fq_mul_si(temp, temp, -5, ctx);
        fq_poly_set_coeff(temp_poly, 2, temp, ctx);

        fq_mul_si(temp, E->a, -4, ctx);
        fq_mul(temp, temp, E->b, ctx);
        fq_poly_set_coeff(temp_poly, 1, temp, ctx);

        fq_pow_ui(temp, E->b, 2, ctx);
        fq_mul_si(temp, temp, -8, ctx);
        fq_pow_ui(temp2, E->a, 3, ctx);
        fq_sub(temp, temp, temp2, ctx);
        fq_poly_set_coeff(temp_poly, 0, temp, ctx);

        fq_set_ui(temp, 4, ctx);
        fq_poly_scalar_mul_fq(temp_poly, temp_poly, temp, ctx);

        list_fq_poly_add(list_psi, temp_poly, ctx);
        fq_clear(temp2, ctx);
    }

    // Cas n >= 5
    fq_poly_t psi_m;
    fq_poly_init(psi_m, ctx);

    fq_t inv2; // 2^{-1} dans F_q
    fq_init(inv2, ctx);
    fq_set_ui(inv2, 2, ctx);
    fq_inv(inv2, inv2, ctx);

    fq_poly_t Weierstrass_equation_2; // (x^3 + ax + b)^2 dans F_q[x]
    fq_poly_init(Weierstrass_equation_2, ctx);
    fq_poly_zero(Weierstrass_equation_2, ctx);
    fq_one(temp, ctx);
    fq_poly_set_coeff(Weierstrass_equation_2, 3, temp, ctx);
    fq_poly_set_coeff(Weierstrass_equation_2, 1, E->a, ctx);
    fq_poly_set_coeff(Weierstrass_equation_2, 0, E->b, ctx);
    fq_poly_mul(Weierstrass_equation_2, Weierstrass_equation_2, Weierstrass_equation_2, ctx);

    for (ulong m = list_fq_poly_len(list_psi); m <= n; m++) {
        ulong j = m / 2;

        if (m % 2 == 0) {
            fq_poly_pow(psi_m, PSI(j-1), 2, ctx);
            fq_poly_mul(psi_m, PSI(j+2), psi_m, ctx);

            fq_poly_pow(temp_poly, PSI(j+1), 2, ctx);
            fq_poly_mul(temp_poly, PSI(j-2), temp_poly, ctx);
                
            fq_poly_sub(psi_m, psi_m, temp_poly, ctx);
            fq_poly_mul(psi_m, PSI(j), psi_m, ctx);

            fq_poly_scalar_mul_fq(psi_m, psi_m, inv2, ctx);
        } else {
            fq_poly_pow(psi_m, PSI(j + (j % 2)), 3, ctx);
            fq_poly_mul(psi_m, PSI(j + 2 - 3*(j % 2)), psi_m, ctx);
            fq_poly_mul(psi_m, psi_m, Weierstrass_equation_2, ctx);

            fq_poly_pow(temp_poly, PSI(j + 1 - (j % 2)), 3, ctx);
            fq_poly_mul(temp_poly, PSI(j - 1 + 3*(j % 2)), temp_poly, ctx);

            if (j % 2 == 0) {
                fq_poly_sub(psi_m, psi_m, temp_poly, ctx);
            } else {
                fq_poly_sub(psi_m, temp_poly, psi_m, ctx);
            }
        }
        
        list_fq_poly_add(list_psi, psi_m, ctx);
    }

    fq_poly_clear(psi_m, ctx);
    fq_clear(temp, ctx);
    fq_clear(inv2, ctx);
    fq_poly_clear(temp_poly, ctx);
    fq_poly_clear(Weierstrass_equation_2, ctx);
}

/**
 * Algorithme de Schoof.
 * c.f Algorithme ??? du rapport.
 */
void ell_schoof(fmpz_t res, const ell_curve_t E, const fq_ctx_t ctx) {
    // Initialisation de A
    fmpz_t A;
    fmpz_init_set_ui(A, 1);

    // Initialisation et définition de q
    fmpz_t q;
    fmpz_init(q);
    fq_ctx_order(q, ctx);

    // Réduction de q modulo l, plus efficace pour calculer [q](x,y) dans E(R_{E,l})
    fmpz_t q_mod_l;
    fmpz_init(q_mod_l);

    // p = car(F_q)
    fmpz_t p;
    fmpz_init(p);
    fmpz_set(p, fq_ctx_prime(ctx));

    // Initialisation de liste des polynômes de division
    list_fq_poly_t list_psi;
    list_fq_poly_init(list_psi);

    // Initialisation de l'anneau de torsion
    tors_ring_t tors_ring;
    tors_ring_init(tors_ring, ctx);

    ell_point_t P, Q, x_y, Frob_x_y, Frob2_x_y;
    ell_point_init(P, ctx);
    ell_point_init(Q, ctx);
    ell_point_init(x_y, ctx);
    ell_point_init(Frob_x_y, ctx);
    ell_point_init(Frob2_x_y, ctx);

    // x_y = (x,y)
    tors_elem_set_x(x_y->X, ctx);
    tors_elem_set_y(x_y->Y, ctx);
    tors_elem_one(x_y->Z, ctx);

    // Frob_x_y et Frob2_x_y seront affines
    tors_elem_one(Frob_x_y->Z, ctx);
    tors_elem_one(Frob2_x_y->Z, ctx);
    
    // list_primes contiendra la liste des nombres premiers pour lesquels on a calculé la classe de a_q
    list_ulong_t list_primes;
    list_ulong_init(list_primes);
    ulong l = 3; // Nombre premier qui passera au suivant à la fin de chaque boucle
    
    // list_ts contiendra la liste des réductions de a_q modulo les nombres_premiers de list_primes
    list_ulong_t list_ts;
    list_ulong_init(list_ts);
    ulong t;

    // A_max = 4*sqrt(q)
    fmpz_t A_max;
    fmpz_init(A_max);
    fmpz_sqrt(A_max, q);
    fmpz_mul_ui(A_max, A_max, 4);

    while (fmpz_cmp(A, A_max) <= 0) {
        if (!fmpz_equal_ui(p, l)) {
            // Calcul de q modulo l
            fmpz_mod_ui(q_mod_l, q, l);

            // On calcule ψ_l
            update_list_div_poly(list_psi, E, l, ctx);

            // Initialisation de l'anneau de torsion
            tors_ring_set(tors_ring, E, list_psi->tail->poly, ctx);

            // Frob_x_y = (x^q, y^q)
            tors_elem_pow(Frob_x_y->X, x_y->X, q, tors_ring, ctx);
            tors_elem_pow(Frob_x_y->Y, x_y->Y, q, tors_ring, ctx);

            // Frob2_x_y = (x^{q^2}, y^{q^2})
            tors_elem_pow(Frob2_x_y->X, Frob_x_y->X, q, tors_ring, ctx);
            tors_elem_pow(Frob2_x_y->Y, Frob_x_y->Y, q, tors_ring, ctx);

            // P = (x^{q^2}, y^{q^2}) + [q](x,y) 
            ell_point_mul(P, x_y, q_mod_l, tors_ring, ctx);
            ell_point_add(P, Frob2_x_y, P, tors_ring, ctx);
            
            for (t = 0; t < l; t++) {
                // Q = [t](x^q,y^q) via [t](x^q,y^q) = [t-1](x^q,y^q) + (x^q,y^q)
                if (t == 0) {
                    ell_point_set_infinity(Q, ctx);
                } else {
                    ell_point_add(Q, Q, Frob_x_y, tors_ring, ctx);
                }

                if (ell_point_equal(P, Q, tors_ring, ctx)) break;
            }

            list_ulong_add(list_ts, t);
            list_ulong_add(list_primes, l);
            fmpz_mul_ui(A, A, l);
        }

        l = n_nextprime(l, 1); // Le 1 en argument signifie que le test de primalité n'est pas probabiliste
    }

    // On utilise le théorème des restes chinois pour retrouver a_q
    fmpz_comb_t comb;
    fmpz_comb_temp_t comb_temp;

    ulong num_primes = list_ulong_len(list_primes);
    ulong* tab_primes = (ulong*)malloc(num_primes * sizeof(ulong));
    ulong* tab_ts = (ulong*)malloc(num_primes * sizeof(ulong));
    list_ulong_get_tab(tab_primes, list_primes, num_primes);
    list_ulong_get_tab(tab_ts, list_ts, num_primes);

    fmpz_comb_init(comb, tab_primes, num_primes);
    fmpz_comb_temp_init(comb_temp, comb);

    fmpz_multi_CRT_ui(res, tab_ts, comb, comb_temp, 1); // Le 1 en argument signifie qu'on prend le représentant canonique signé

    // On attribue à res la valeur q + 1 - a_q
    fmpz_sub(res, q, res);
    fmpz_add_ui(res, res, 1);

    // Libération de la mémoire
    fmpz_clear(A);
    fmpz_clear(q);
    fmpz_clear(A_max);

    tors_ring_clear(tors_ring, ctx);

    ell_point_clear(P, ctx);
    ell_point_clear(Q, ctx);
    ell_point_clear(x_y, ctx);
    ell_point_clear(Frob_x_y, ctx);
    ell_point_clear(Frob2_x_y, ctx);

    list_ulong_clear(list_primes);
    list_ulong_clear(list_ts);
    list_fq_poly_clear(list_psi, ctx);

    free(tab_primes);
    free(tab_ts);

    fmpz_comb_clear(comb);
    fmpz_comb_temp_clear(comb_temp);
}


/**
 * Exécute l'algorithme de Schoof avec seulement les paramètres de la courbe en entrée.
 * C'est cette fonction à laquelle il faut faire appel si on importe cette bibliothèque.
 * 
 * Renvoie EXIT_SUCCESS si les paramètres vérifient les conditions demandées (lissité de la courbe et corps
 * de base de caractéristique différente de 2 et 3), laisse inchangée la sortie et renvoie EXIT_FAILURE sinon.
 */
int schoof(fmpz_t res, const fq_t a, const fq_t b, const fq_ctx_t ctx) {
    ell_curve_t E;
    ell_curve_init(E, ctx);

    int success = EXIT_SUCCESS;

    fmpz_t p;
    fmpz_init(p);
    fmpz_set(p, fq_ctx_prime(ctx));

    if (fmpz_equal_ui(p, 2) || fmpz_equal_ui(p, 3)) {
        success = EXIT_FAILURE;
    } else {
        if (ell_curve_set(E, a, b, ctx) == EXIT_SUCCESS) {
            ell_schoof(res, E, ctx);
        } else {
            success = EXIT_FAILURE;
        }
    }

    fmpz_clear(p);
    ell_curve_clear(E, ctx);
    return success;
}