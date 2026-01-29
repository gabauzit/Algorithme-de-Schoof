#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <flint/flint.h>
#include <flint/fq_poly.h>

// On peut pas faire d'allocation dynamique avec malloc, à moins de connaitre à l'avance le nombre de premiers
// nécessaires pour pouvoir reconstituer a_q, et ceci demanderait des estimations précises sur la fonction
// de Tchebychev de première espèce. On peut faire beaucoup plus simple avec des listes, d'autant qu'en général 
// on n'a pas besoin de beaucoup de nombres premiers donc accéder à un élément de la liste ne sera pas très couteux.


/**
 * Il y a deux possibilités : créer une structure très générale de liste qui peut contenir plusieurs types, mais
 * il faut définir des pointeurs de fonctions pour savoir comment initialiser la mémoire, la libérer, etc...
 * C'est trop compliqué pour notre situation, d'autant qu'on a besoin de seulement deux types.
 * 
 * A l'autre extrême, on peut créer deux structures différentes, une pour chaque type, mais il y aurait beaucoup
 * de duplication de code.
 * 
 * Il a été décidé d'adopter un entre-deux : on sépare les responsabilités. Le type list_t est complètement
 * générique et ne sait pas quel type de donnés il stocke, il sait seulement gérer la mémoire. Chaque type
 * aura des fonctions dédiées pour récupérer ou rajouter un élément à la liste. Enfin, chaque fonction qui
 * utilise le type list_t saura dans quel cadre il se trouve et donc quelle fonction appeler. 
 */

/**
 * Il vaut mieux faire deux types séparés, sinon on rentre dans un niveau d'abstraction absurde sachant qu'on
 * veut seulement stocker deux types (et jamais changer au sein d'une même liste).
 */

/*************************************************/
/* DEFINITION ET PRIMITIVES DU TYPE list_ulong_t */
/*************************************************/

typedef struct cell_ulong_struct {
    ulong index; // Numéro de la cellule
    ulong t; // Sera la réduction modulo prime de a_q
    struct cell_ulong_struct *next;
} cell_ulong_t;

typedef struct {
    cell_ulong_t *head;
    cell_ulong_t *tail; // Fin de la liste, beaucoup plus efficace pour rajouter des éléments en fin de liste.
} list_ulong_struct;

typedef list_ulong_struct list_ulong_t[1]; // On adopte la convention de FLINT sur les nouveaux types

void list_ulong_init(list_ulong_t);
void list_ulong_clear(list_ulong_t);
ulong list_ulong_len(const list_ulong_t);
void list_ulong_add(list_ulong_t, const ulong);
ulong list_ulong_get(list_ulong_t, const ulong);
void list_ulong_get_tab(ulong*, const list_ulong_t, const ulong);

/***************************************************/
/* DEFINITION ET PRIMITIVES DU TYPE list_fq_poly_t */
/***************************************************/

typedef struct cell_fq_poly_struct {
    ulong index; // Numéro de la cellule
    fq_poly_t poly;
    struct cell_fq_poly_struct* next;
} cell_fq_poly_t;

typedef struct {
    cell_fq_poly_t* head;
    cell_fq_poly_t* tail; // Fin de la liste, beaucoup plus efficace pour rajouter des éléments en fin de liste.
} list_fq_poly_struct;

typedef list_fq_poly_struct list_fq_poly_t[1]; // On adopte la convention de FLINT sur les nouveaux types

void list_fq_poly_init(list_fq_poly_t);
void list_fq_poly_clear(list_fq_poly_t, const fq_ctx_t);
ulong list_fq_poly_len(const list_fq_poly_t);
void list_fq_poly_add(list_fq_poly_t, const fq_poly_t, const fq_ctx_t ctx);
fq_poly_struct* list_fq_poly_get(list_fq_poly_t, const ulong);

#endif