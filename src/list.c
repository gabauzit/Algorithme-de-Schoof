#include "list.h"

/***********************************/
/* PRIMITIVES DU TYPE list_ulong_t */
/***********************************/

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

/**
 * Attention : par rapport à son équivalent pour le type list_ulong_t ici l'élément à rajouter n'est pas
 * immuable à cause de fq_poly_swap(). Cette fonction sera appelée uniquement pour le calcul des polynômes de
 * division auquel cas on rajoutera une variable qui était initialement temporaire, ceci évite une copie coûteuse.
 */
void list_fq_poly_add(list_fq_poly_t list, fq_poly_t poly, const fq_ctx_t ctx) {
    cell_fq_poly_t* ptr = (cell_fq_poly_t*)malloc(sizeof(cell_fq_poly_t));
    fq_poly_init(ptr->poly, ctx);
    fq_poly_swap(ptr->poly, poly, ctx);
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
 * Renvoie l'élément de list d'indice index. Si list est de taille inférieure à index, génère une erreur.
 */
fq_poly_struct* list_fq_poly_get(const list_fq_poly_t list, const ulong index) {
    cell_fq_poly_t* ptr = list->head;

    while(ptr != NULL && ptr->index != index) {
        ptr = ptr->next;
    }

    if (ptr == NULL) {
        fprintf(stderr, "Error: invalid index\n");
        abort();
    }

    return ptr->poly;
}