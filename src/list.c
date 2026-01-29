#include "list.h"

/***********************************/
/* PRIMITIVES DU TYPE list_ulong_t */
/***********************************/

/**
 * Initialise une liste de type list_ulong_t. Il n'y a pas besoin d'allocation dynamique car, selon la
 * convention des nouveaux types de FLINT adoptée ici, list_ulong_t est un tableau de taille 1 donc la
 * mémoire est allouée dès la déclaration de la liste.
 */
void list_ulong_init(list_ulong_t list) {
    list->head = NULL;
    list->tail = NULL;
}

void list_ulong_clear(list_ulong_t list) {
    cell_ulong_t* ptr = list->head;
    cell_ulong_t* next;

    while (ptr != NULL) {
        next = ptr->next;
        free(ptr);
        ptr = next;
    }

    list->head = NULL;
    list->tail = NULL;
}

ulong list_ulong_len(const list_ulong_t list) {
    return (list->tail == NULL) ? 0 : list->tail->index + 1;
}

void list_ulong_add(list_ulong_t list, const ulong t) {
    cell_ulong_t* ptr = (cell_ulong_t*)malloc(sizeof(cell_ulong_t));
    ptr->t = t;
    ptr->next = NULL;

    if (list->head == NULL) {
        ptr->index = 0;
        list->head = ptr;
        list->tail = ptr;
    } else {
        ptr->index = list->tail->index + 1;
        list->tail->next = ptr;
        list->tail = ptr;
    }
}

/**
 * Renvoie l'élément liste d'indice index. Si index est plus grand que la longueur de la liste, renvoie
 * le dernier élément de la liste.
 * GESTION D'ERREUR SI LA LISTE EST VIDE ???
 */
ulong list_ulong_get(list_ulong_t list, const ulong index) {
    cell_ulong_t* ptr = list->head;

    while(ptr != NULL && ptr->index != index) {
        ptr = ptr->next;
    }

    return ptr->t;
}

/**
 * Convertit les len premiers éléments de la liste dans le tableau tab. Si list contient moins de len éléments,
 * toute la liste est copiée dans tab.
 */
void list_ulong_get_tab(ulong* tab, const list_ulong_t list, const ulong len) {
    cell_ulong_t* ptr = list->head;

    for (ulong i = 0; i < len && ptr != NULL; i++) {
        tab[i] = ptr->t;
        ptr = ptr->next;
    }
}

/*************************************/
/* PRIMITIVES DU TYPE list_fq_poly_t */
/*************************************/

/**
 * Initialise une liste de type list_fq_poly_t. Il n'y a pas besoin d'allocation dynamique car, selon la
 * convention des nouveaux types de FLINT adoptée ici, list_fq_poly_t est un tableau de taille 1 donc la
 * mémoire est allouée dès la déclaration de la liste.
 */
void list_fq_poly_init(list_fq_poly_t list) {
    list->head = NULL;
    list->tail = NULL;
}

void list_fq_poly_clear(list_fq_poly_t list, const fq_ctx_t ctx) {
    cell_fq_poly_t* ptr = list->head;
    cell_fq_poly_t* next;
    
    while (ptr != NULL)
    {
        next = ptr->next;
        fq_poly_clear(ptr->poly, ctx);
        free(ptr);
        ptr = next;
    }
    
    list->head = NULL;
    list->tail = NULL;
}

ulong list_fq_poly_len(const list_fq_poly_t list) {
    return (list->tail == NULL) ? 0 : list->tail->index + 1;
}

void list_fq_poly_add(list_fq_poly_t list, const fq_poly_t fq_poly, const fq_ctx_t ctx) {
    cell_fq_poly_t* ptr = (cell_fq_poly_t*)malloc(sizeof(cell_fq_poly_t));
    fq_poly_init(ptr->poly, ctx);
    fq_poly_set(ptr->poly, fq_poly, ctx);
    ptr->next = NULL;
    
    if (list->head == NULL) {
        ptr->index = 0;
        list->head = ptr;
        list->tail = ptr;
    } else {
        ptr->index = list->tail->index + 1;
        list->tail->next = ptr;
        list->tail = ptr;
    }
}

/**
 * Renvoie l'élément liste d'indice index. Si index est plus grand que la longueur de la liste, renvoie
 * le dernier élément de la liste.
 * GESTION D'ERREUR SI list EST VIDE ???
 */
fq_poly_struct* list_fq_poly_get(list_fq_poly_t list, const ulong index) {
    cell_fq_poly_t* ptr = list->head;

    while(ptr != NULL && ptr->index != index) {
        ptr = ptr->next;
    }

    return ptr->poly;
}