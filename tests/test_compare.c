#include "test_compare.h"

/**
 * On utilise #E(F_q) = q + 1 + \sum_{x\in F_q} is_square(x^3+ax+b, q), q est premier
 */
void naive_num_of_points(fmpz_t res, const fq_t a, const fq_t b, const fq_ctx_t ctx) {
    fmpz_t q;
    fmpz_init(q);
    fq_ctx_order(q, ctx);

    fmpz_set(res, q);
    fmpz_add_ui(res, res, 1);

    fq_t x, temp1, temp2;
    fq_init(x, ctx);
    fq_init(temp1, ctx);
    fq_init(temp2, ctx);

    fmpz_t i;
    fmpz_init(i);

    for(fmpz_zero(i); fmpz_cmp(q, i) > 0; fmpz_add_ui(i, i, 1)) {
        fq_set_fmpz(x, i, ctx);
        fq_pow_ui(temp1, x, 3, ctx);
        fq_mul(temp2, a, x, ctx);
        fq_add(temp1, temp1, temp2, ctx);
        fq_add(temp1, temp1, b, ctx);

        if (!fq_is_zero(temp1, ctx)) fmpz_add_si(res, res, 2*fq_is_square(temp1, ctx) - 1);
    }

    fmpz_clear(q);
    fmpz_clear(i);
    fq_clear(x, ctx);
    fq_clear(temp1, ctx);
    fq_clear(temp2, ctx);
}

int main() {    
    FILE* file = fopen("./results/results_compare.csv", "w");
    fprintf(file, "NUM_TRIALS, MIN_BITS, MAX_BITS\n");
    fprintf(file, "%i, %i, %i\n", NUM_TRIALS, MIN_BITS, MAX_BITS);
    fprintf(file, "q,a,b,naive,schoof\n"); // Format du fichier .csv

    flint_rand_t state;
    flint_randinit(state);

    fmpz_t q;
    fmpz_init(q);

    fmpz_t res_schoof, res_naive;
    fmpz_init(res_schoof);
    fmpz_init(res_naive);
    
    int num_of_success = 0;

    for (int i = MIN_BITS; i <= MAX_BITS; i++) {
        for (int j = 0; j < NUM_TRIALS; j++) { 
            fmpz_randprime(q, state, i, 1);

            fq_ctx_t ctx;
            fq_ctx_init(ctx, q, 1, "a");

            fq_t a, b;
            fq_init(a, ctx);
            fq_init(b, ctx);
            
            // Ecriture de q
            fmpz_fprint(file, q);
            fprintf(file, ",");
            
            // On tire une courbe elliptique au hasard (on vérifie que les paramètres tirés la rendent lisse)
            int essais_max = 10;
            do {
                fq_rand(a, state, ctx);
                fq_rand(b, state, ctx);
            } while ((essais_max--) > 0 && schoof(res_schoof, a, b, ctx) == EXIT_FAILURE); // On évite les boucles infinies potentielles

            // Ecriture de a et b
            fq_fprint_pretty(file, a, ctx);
            fprintf(file, ",");
            fq_fprint_pretty(file, b, ctx);
            fprintf(file, ",");
            
            naive_num_of_points(res_naive, a, b, ctx);
            num_of_success += fmpz_equal(res_schoof, res_naive);

            // Ecriture de res_naive et res_schoof
            fmpz_fprint(file, res_naive);
            fprintf(file, ",");
            fmpz_fprint(file, res_schoof);
            fprintf(file, "\n");

            fq_clear(a, ctx);
            fq_clear(b, ctx);
            fq_ctx_clear(ctx);
        }
        printf("Tests sur %i bits terminés.\n", i);
    }

    fmpz_clear(res_schoof);
    fmpz_clear(res_naive);
    fmpz_clear(q);
    flint_randclear(state);

    printf("\nTests réussis : %i / %i.\n", num_of_success, TOTAL_NUM_TRIALS);

    if (num_of_success < TOTAL_NUM_TRIALS) {
        printf("⚠️ %i tests ont échoués\n", TOTAL_NUM_TRIALS - num_of_success);
    } else {
        printf("🎉 Tous les tests ont réussis ! 🎉\n");
    }
    fclose(file);
}