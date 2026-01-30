# Algorithme de Schoof

# Commandes disponibles

Ouvrir un terminal dans le repértoire du projet.

`make` ou `make all` Compile le projet

`make test-quick` Tests rapides (3 essais, 8-16 bits)

`make test NUM_TRIALS MIN_BITS MAX_BITS` Éxécute les tests (5 essais, 8-32 bits)

`make clean` Supprime les fichiers objets et exécutables

`make help` Affiche cette aide

# Descriptif des dossiers

- `bin/` Contient l'exécutable des tests

- `include/` Contient les fichiers d'en-tête .h

- `obj/` Contient les fichiers objets .o

- `results/` Contient les résultats des tests

- `src/` Contient les fichiers sources .c

- `tests/` Contient les fichiers de tests

# Descriptif des fichiers

- **ell_curve.h** & **ell_curve.c** : Définition du type **ell_curve_t** et ses primitives.

- **torsion_ring.h** & **torsion_ring.c** : Définitions des types **tors_ring_t** et **tors_elem_t** et leurs primitives, ainsi que l'arithmétique dans les anneaux de torsion.

- **ell_point.h** & **ell_point.c** : Définition du type **ell_point_t** et ses primitives, ainsi que les opérations issues de la loi de groupe sur les courbes elliptiques. Les points d'une courbe elliptique sont à coefficients dans un anneau de torsion.

- **schoof.h** & **schoof.c** : Algorithme de Schoof et génération des polynômes de division.

- **test_compare.h** & **test_compare.c** : Compare les résultats donnés par l'algorithme de Schoof avec une méthode directe mais moins efficace sur des entrées aléatoires.

# A FAIRE :

- S'assurer qu'on a inclus toutes les bibliothèques FLINT qu'on a utlisées dans chaque header.

- Rajouter des explications en commentaires dans chaque fichier .h pour connaitre son role.

- Identification du goulot d'étranglement.

- Rajouter des références de certains algorithmes vers certaines propositions du rapport.

- Vérifier si, pour la lisibilité, il n'y a pas trop de sauts de lignes.

- Gestion d'erreur dans list.c dans les fonctions get() si la liste est vide. Par exemple
```
if (ptr == NULL) {
    fprintf(stderr, "Error: invalid index\n");
    abort();
}
```
mais dans ce cas il faut inclure <stdio.h> et <stdlib.h>.

- Je déclare parfois des variables qui ne sont utiles que dans une boucle future, il vaut mieux éviter car
ça copie cette variable à chaque boucle, autant la définir dans la boucle.

- Changer flint_randinit() et flint_randclear() qui sont visiblement obsolètes. Par quoi ??

- On peut remplacer des init() puis zero() par init2() à certains endroits ?

- Vérifier si certaines primitives ou fonctions ne sont pas utilisées

- Duplication de code dans la récurrence des psi_m pour m impair ?

- CHANGER LES fq_set() EN fq_poly_swap(), IDEM POUR fq_poly_t

- Rajouter des condition rop != op dans les fonctions qui évitent l'aliasing par des copies, ça permet d'éviter des lourdes copies inutiles, ou juste faire un fq_swap() plutot

- Problème dans le makefile, `make` affiche les paramètres de tests alors que ça devrait pas.

Commande pour gprof :
```
# Compiler avec -pg
make clean
make CFLAGS="-Wall -Wextra -O2 -std=c11 -Iinclude -Wno-deprecated-declarations -pg" \
     LDFLAGS="-lflint -lgmp -lmpfr -pg" \
     NUM_TRIALS=1000 MIN_BITS=16 MAX_BITS=32 \
     test-perf

# Exécuter
./bin/test_perf

# Analyser
gprof ./bin/test_perf gmon.out > profile_report.txt
cat profile_report.txt
```
Penser à créer tous les fichiers objets et exécutables avant de commit