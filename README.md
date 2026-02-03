# Algorithme de Schoof

Ce repértoire contient une implémentation en C de l'algorithme de Schoof à l'aide de la librairie FLINT. Le fichier **rapport.pdf** est un rapport expliquant l'algorithme et ce programme.

# Commandes disponibles

Ouvrir un terminal dans le repértoire du projet.

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