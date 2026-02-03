#include "test_perf.h"

int main() {    
    FILE* file = fopen("./results/results_perf.csv", "w");
    fprintf(file, "NUM_TRIALS,MIN_BITS,MAX_BITS\n");
    fprintf(file, "%i,%i,%i\n", NUM_TRIALS, MIN_BITS, MAX_BITS);
    fprintf(file, "q,a,b,time (s)\n"); // Format du fichier .csv

    flint_rand_t state;
    flint_randinit(state);

    fmpz_t q;
    fmpz_init(q);

    fmpz_t res;
    fmpz_init(res);

    clock_t start, end;
    double duration;

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
            int success;
            do {
                fq_rand(a, state, ctx);
                fq_rand(b, state, ctx);

                start = clock();
                success = schoof(res, a, b, ctx); // Pour vérifier si les paramètres étaient corrects
                end = clock();
            } while ((essais_max--) > 0 && success == EXIT_FAILURE); // On évite les potentielles boucles infinies

            duration = (double)(end - start) / CLOCKS_PER_SEC;

            // Ecriture de a et b
            fq_fprint_pretty(file, a, ctx);
            fprintf(file, ",");
            fq_fprint_pretty(file, b, ctx);
            fprintf(file, ",");

            // Ecriture du temps de calcul
            fprintf(file, "%.6f\n", duration);

            fq_clear(a, ctx);
            fq_clear(b, ctx);
            fq_ctx_clear(ctx);
        }
        printf("Tests sur %i bits terminés.\n", i);
    }

    fmpz_clear(q);
    fmpz_clear(res);
    flint_randclear(state);

    printf("\n 🎉 Tests terminés ! 🎉\n");

    fclose(file);

    printf("\n 📊 Génération du graphique...\n");

    if (system("python results/graph_results_perf.py") != 0) {
        fprintf(stderr, "Erreur lors de l'exécution du script Python.\n");
    }


}