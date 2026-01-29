#include "schoof.h"

/**
 * Actualise list_psi en calculant tous les ψ_m encore non-calculés jusqu'à ψ_n.
 */
void update_list_div_poly(list_fq_poly_t list_psi, const ell_curve_t E, const ulong n, const fq_ctx_t ctx) {
    cell_fq_poly_t* last_psi_cell = list_psi->tail;
    slong last_psi_cell_index = (last_psi_cell == NULL) ? -1 : (slong)last_psi_cell->index;
    
    fq_t temp;
    fq_init(temp, ctx);

    fq_poly_t temp_psi;
    fq_poly_init(temp_psi, ctx);

    // ψ_0 = 0
    if (n == 0 || (n > 0 && last_psi_cell_index < 0)) { // Si n = 0 ou (n > 0 et ψ_0 n'a pas encore été calculé)
        fq_poly_zero(temp_psi, ctx);
        list_fq_poly_add(list_psi, temp_psi, ctx);
    }

    // ψ_1 = 1
    if (n == 1 || (n > 1 && last_psi_cell_index  < 1)) { // Si n = 1 ou (n > 1 et ψ_1 n'a pas encore été calculé)
        fq_poly_one(temp_psi, ctx);
        list_fq_poly_add(list_psi, temp_psi, ctx);
    }

    // ψ_2 = 2*y
    if (n == 2 || (n > 2 && last_psi_cell_index < 2)) { // Si n = 2 ou (n > 2 et ψ_2 n'a pas encore été calculé)
        fq_poly_zero(temp_psi, ctx);
        fq_set_ui(temp, 2, ctx);
        fq_poly_set_fq(temp_psi, temp, ctx);
        list_fq_poly_add(list_psi, temp_psi, ctx);
    }

    // ψ_3 = 3*x^4 + 6*a*x^2 + 12*b*x - a^2
    if (n == 3 || (n > 3 && last_psi_cell_index < 3)) { // Si n = 3 ou (n > 3 et ψ_3 n'a pas encore été calculé)
        fq_poly_zero(temp_psi, ctx);
        
        fq_set_ui(temp, 3, ctx);
        fq_poly_set_coeff(temp_psi, 4, temp, ctx);

        fq_set_ui(temp, 6, ctx);
        fq_mul(temp, temp, E->a, ctx);
        fq_poly_set_coeff(temp_psi, 2, temp, ctx);

        fq_set_ui(temp, 12, ctx);
        fq_mul(temp, temp, E->b, ctx);
        fq_poly_set_coeff(temp_psi, 1, temp, ctx);

        fq_pow_ui(temp, E->a, 2, ctx);
        fq_neg(temp, temp, ctx);
        fq_poly_set_coeff(temp_psi, 0, temp, ctx);

        list_fq_poly_add(list_psi, temp_psi, ctx);
    }

    // ψ_4 = 4*y*(x^6 + 5*a*x^4 + 20*b*x^3 - 5*a^2*x^2 - 4*a*b*x - 8*b^2 - a^3)
    if (n == 4 || (n > 4 && last_psi_cell_index < 4)) { // Si n = 4 ou (n > 4 et ψ_4 n'a pas encore été calculé)
        fq_t temp2;
        fq_init(temp2, ctx);
        fq_poly_zero(temp_psi, ctx);

        fq_set_ui(temp, 1, ctx);
        fq_poly_set_coeff(temp_psi, 6, temp, ctx);

        fq_set_ui(temp, 5, ctx);
        fq_mul(temp, temp, E->a, ctx);
        fq_poly_set_coeff(temp_psi, 4, temp, ctx);

        fq_set_ui(temp, 20, ctx);
        fq_mul(temp, temp, E->b, ctx);
        fq_poly_set_coeff(temp_psi, 3, temp, ctx);

        fq_set_si(temp, -5, ctx);
        fq_mul(temp, temp, E->a, ctx);
        fq_mul(temp, temp, E->a, ctx);
        fq_poly_set_coeff(temp_psi, 2, temp, ctx);

        fq_set_si(temp, -4, ctx);
        fq_mul(temp, temp, E->a, ctx);
        fq_mul(temp, temp, E->b, ctx);
        fq_poly_set_coeff(temp_psi, 1, temp, ctx);

        fq_set_si(temp, -8, ctx);
        fq_mul(temp, temp, E->b, ctx);
        fq_mul(temp, temp, E->b, ctx);
        fq_pow_ui(temp2, E->a, 3, ctx);
        fq_sub(temp, temp, temp2, ctx);
        fq_poly_set_coeff(temp_psi, 0, temp, ctx);

        fq_set_ui(temp, 4, ctx);
        fq_poly_scalar_mul_fq(temp_psi, temp_psi, temp, ctx);

        list_fq_poly_add(list_psi, temp_psi, ctx);
        fq_clear(temp2, ctx);
    }

    // Cas n >= 5
    for (ulong m = list_fq_poly_len(list_psi); m <= n; m++) {
        ulong j = m / 2;

        fq_poly_t psi_m;
        fq_poly_init(psi_m, ctx);

        if (m % 2 == 0) {           
            // On utilise la récurrence ψ_{2j} = (ψ_j/2y)(ψ_{j+2}ψ_{j-1}^2 - ψ_{j-2}ψ_{j+1}^2)
            fq_poly_pow(psi_m, PSI(j-1), 2, ctx);
            fq_poly_mul(psi_m, PSI(j+2), psi_m, ctx);

            fq_poly_pow(temp_psi, PSI(j+1), 2, ctx);
            fq_poly_mul(temp_psi, PSI(j-2), temp_psi, ctx);
                
            fq_poly_sub(psi_m, psi_m, temp_psi, ctx);
            fq_poly_mul(psi_m, PSI(j), psi_m, ctx);

            fq_set_ui(temp, 2, ctx);
            fq_inv(temp, temp, ctx);
            fq_poly_scalar_mul_fq(psi_m, psi_m, temp, ctx);
        } else {
            // temp_psi = (x^3 + a*x + b)^2
            fq_poly_zero(temp_psi, ctx);
            fq_one(temp, ctx);
            fq_poly_set_coeff(temp_psi, 3, temp, ctx);
            fq_poly_set_coeff(temp_psi, 1, E->a, ctx);
            fq_poly_set_coeff(temp_psi, 0, E->b, ctx);
            fq_poly_mul(temp_psi, temp_psi, temp_psi, ctx);

            if (j % 2 == 0) {
                // On utilise la formule de récurrence ...
                fq_poly_pow(psi_m, PSI(j), 3, ctx);
                fq_poly_mul(psi_m, PSI(j+2), psi_m, ctx);
                fq_poly_mul(psi_m, psi_m, temp_psi, ctx);

                fq_poly_pow(temp_psi, PSI(j+1), 3, ctx);
                fq_poly_mul(temp_psi, PSI(j-1), temp_psi, ctx);

                fq_poly_sub(psi_m, psi_m, temp_psi, ctx);
            } else {
                // On utilise la formule de récurrence ...
                fq_poly_pow(psi_m, PSI(j+1), 3, ctx);
                fq_poly_mul(psi_m, PSI(j-1), psi_m, ctx);
                fq_poly_mul(psi_m, psi_m, temp_psi, ctx);

                fq_poly_pow(temp_psi, PSI(j), 3, ctx);
                fq_poly_mul(temp_psi, PSI(j+2), temp_psi, ctx);

                fq_poly_sub(psi_m, temp_psi, psi_m, ctx);
            }
        }
        
        list_fq_poly_add(list_psi, psi_m, ctx);
        last_psi_cell = list_psi->tail;
        fq_poly_clear(psi_m, ctx);
    }

    fq_clear(temp, ctx);
    fq_poly_clear(temp_psi, ctx);
}

// Peut être qu'il faut enlever 2 du fait que c'est un cas à part, auquel cas il faudra peut etre
// rajouter un nombre premier à la liste. Le fait que psi_2 = 2*y empeche d'avoir des représentants
// canoniques pour x et y dans l'anneau de torsion, il faut donc l'enlever.

// A <= 4*sqrt(q) iff log_2(A) <= 2 + log_2(q)/2, or 2 + log_2(q)/2 <= 2 + (nbre_bits(q))/2 = (4+n_bits(q))/2
// donc si 2*log_2(A) > 4 + n_bits(q), alors A > 2^(2 + log_2(q)/2) = 4*sqrt(q).
// log_2(A) >= floor(log_2(A)) = n_bits(A) - 1 donc,
// si 2*n_bits(A) > 6 + n_bits(q), alors 2*log_2(A) > 2*n_bits(A) - 2 >= 4 + n_bits(q)

void ell_schoof(fmpz_t res, const ell_curve_t E, const fq_ctx_t ctx) {
    // Initialisation de A
    fmpz_t A;
    fmpz_init_set_ui(A, 1);

    // Initialisation et définition de q
    fmpz_t q;
    fmpz_init(q);
    fq_ctx_order(q, ctx);

    // Calcul de p (la caractéristique de F_q)
    fmpz_t p;
    fmpz_init(p);
    fmpz_set(p, fq_ctx_prime(ctx));

    // Initialisation du tableau des polynômes de division
    list_fq_poly_t list_psi;
    list_fq_poly_init(list_psi);

    // Initialisation de l'anneau de torsion R_{ψ_l}
    tors_ring_t tors_ring;
    tors_ring_init(tors_ring, ctx);

    // Définition des monômes x et y dans l'anneau de torsion
    tors_elem_t x, y;
    tors_elem_inits(ctx, x, y, NULL);
    tors_elem_set_x(x, ctx);
    tors_elem_set_y(y, ctx);

    // On aura P = (x^{q^2}, y^{q^2}) + [q](x,y), Q = [n](x^q, y^q) et on vérifiera si P == Q
    ell_point_t P, Q, temp1, temp2;
    ell_point_init(P, ctx);
    ell_point_init(Q, ctx);
    ell_point_init(temp1, ctx);
    ell_point_init(temp2, ctx);

    // list_primes contiendra la liste des nombres premiers pour lesquels on a calculé la classe de a_q
    list_ulong_t list_primes;
    list_ulong_init(list_primes);
    ulong l = 3; // Nombre premier qui passera au suivant à la fin de chaque boucle
    
    // list_ts contiendra la liste des réduction de a_q modulo les nombres_premiers de list_primes
    list_ulong_t list_ts;
    list_ulong_init(list_ts);
    ulong t;

    // Permet de savoir si on a trouvé le bon résidu modulo un nombre premier fixé
    int success;

    ulong A_max_bits = (8 + fmpz_bits(q)) / 2;

    while (fmpz_bits(A) < A_max_bits) {
        if (!fmpz_equal_ui(p, l)) { // On vérifie que l est différent de la caractéristique de F_q
            success = 0;

            // On calcule puis récupère ψ_l
            update_list_div_poly(list_psi, E, l, ctx);
            fq_poly_t psi_l;
            fq_poly_init(psi_l, ctx);
            fq_poly_set(psi_l, list_psi->tail->poly, ctx);

            // Initialisation de l'anneau de torsion
            tors_ring_set(tors_ring, E, psi_l, ctx);

            t = 0;
            while (!success && t < l) { // En théorie, on pourrait enlever la condition t < l mais je préfère m'assurer qu'il n'y a pas de boucle infinie                               
                // temp1 = (x,y)
                tors_elem_copy(temp1->X, x, ctx);
                tors_elem_copy(temp1->Y, y, ctx);
                tors_elem_one(temp1->Z, ctx);

                // temp2 = (x^q, y^q)
                tors_elem_pow(temp2->X, x, q, tors_ring, ctx);
                tors_elem_pow(temp2->Y, y, q, tors_ring, ctx);
                tors_elem_one(temp2->Z, ctx);

                // Calcul de Q = [t](x^q,y^q) via [t](x^q,y^q) = [t-1](x^q,y^q) + (x^q,y^q)
                if (t == 0) {
                    ell_point_set_infinity(Q, ctx);
                } else {
                    ell_point_add(Q, Q, temp2, tors_ring, ctx);
                }

                // Calcul de P = (x^{q^2}, y^{q^2}) + [q](x,y)
                tors_elem_pow(P->X, temp2->X, q, tors_ring, ctx);
                tors_elem_pow(P->Y, temp2->Y, q, tors_ring, ctx);
                tors_elem_one(P->Z, ctx);
                ell_point_mul(temp1, temp1, q, tors_ring, ctx);
                ell_point_add(P, P, temp1, tors_ring, ctx);

                if (ell_point_equal(P, Q, tors_ring, ctx)) {
                    success = 1;
                } else {
                    t++;
                }
            }

            list_ulong_add(list_ts, t);
            list_ulong_add(list_primes, l);

            fmpz_mul_ui(A, A, l);
            fq_poly_clear(psi_l, ctx);
        }
        l = n_nextprime(l, 1);
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

    fmpz_multi_CRT_ui(res, tab_ts, comb, comb_temp, 1); // Le 1 signifie qu'on prend le représentant canonique signé

    // On attribue à res la valeur q + 1 - a_q
    fmpz_sub(res, q, res);
    fmpz_add_ui(res, res, 1);

    // Libération de la mémoire allouée
    fmpz_clear(A);
    fmpz_clear(q);

    list_fq_poly_clear(list_psi, ctx);

    tors_ring_clear(tors_ring, ctx);

    tors_elem_clears(ctx, x, y, NULL);

    ell_point_clear(P, ctx);
    ell_point_clear(Q, ctx);
    ell_point_clear(temp1, ctx);
    ell_point_clear(temp2, ctx);

    list_ulong_clear(list_primes);
    list_ulong_clear(list_ts);

    free(tab_primes);
    free(tab_ts);

    fmpz_comb_clear(comb);
    fmpz_comb_temp_clear(comb_temp);
}


/**
 * Exécute l'algorithme de Schoof avec seulement les paramètres de la courbe en entrée.
 * C'est cette fonction à laquelle il faut faire appel si on importe cette bibliothèque.
 * 
 * Renvoie EXIT_SUCCESS si les paramètres vérifient les conditions demandées (lissité de la courbe),
 * EXIT_FAILURE sinon.
 */
int schoof(fmpz_t res, const fq_t a, const fq_t b, const fq_ctx_t ctx) {
    ell_curve_t E;
    int success = EXIT_SUCCESS;

    fmpz_t p;
    fmpz_init(p);
    fmpz_set(p, fq_ctx_prime(ctx));

    if (fmpz_equal_ui(p, 2) || fmpz_equal_ui(p, 3)) {
        fmpz_clear(p);
        return EXIT_FAILURE;
    }

    if (ell_curve_init_set(E, a, b, ctx) == EXIT_SUCCESS) {
        ell_schoof(res, E, ctx);
        ell_curve_clear(E, ctx);
    } else {
        success = EXIT_FAILURE;
    }

    return success;
}