# Algorithme de Schoof

Ce repértoire contient une implémentation en C de l'algorithme de Schoof à l'aide de la librairie FLINT. Le fichier **rapport.pdf** est un court rapport expliquant l'algorithme de façon théorique et les idées d'implémentation que l'on retrouve dans ce programme.

Pour utiliser soi-même l'algorithme, il suffit de créer un fichier `.c` dans le même répertoire que ce programme, d'inclure les dépendances FLINT et le fichier d'en-tête `schoof.h` puis de faire appel à la fonction `schoof()` dont la définition est :

```C
int schoof(fmpz_t res, const fq_t a, const fq_t b, const fq_ctx_t ctx);
/**
 * Exécute l'algorithme de Schoof pour la courbe elliptique d'équation y^2 = x^3 + a*x + b.
 * 
 * Si les paramètres vérifient les hypothèses nécessaires, à savoir :
 *    - le corps de base est de caractéristique différente de 2 et 3,
 *    - la courbe est lisse, c'est-à-dire 4*a^3+27*b^2 est non-nul,
 * alors affecte à res le nombre de points de la courbe elliptique et renvoie EXIT_SUCCESS.
 * Dans le cas contraire, laisse les arguments inchangés et renvoie EXIT_FAILURE.
 */
```

# Commandes disponibles

Ouvrir un terminal dans le repértoire du projet et saisir l'une des commandes suivantes :

`make` ou `make all` Compile le projet

`make test-compare` Comparaison avec une méthode naïve

`make test-perf` Mesure le temps d'exécution

`make clean` Supprime les fichiers objets et exécutables

`make help` Affiche cette aide

**Paramètres personnalisables :**

`NUM_TRIALS` Nombre d'essais pas taille

`MIN_BITS` Taille minimale en bits

`MAX_BITS` Taille maximale en bits

**Exemples :**

`make test-compare NUM_TRIALS=2 MIN_BITS=16 MAX_BITS=32`

`make test-perf NUM_TRIALS=10 MIN_BITS=16 MAX_BITS=64`

**Note** : `MIN_BITS` doit valoir au moins 4.