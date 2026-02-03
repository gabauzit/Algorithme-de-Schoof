#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <flint/flint.h>
#include <flint/fq_poly.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Section 3.3 du rapport
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
void list_fq_poly_add(list_fq_poly_t, fq_poly_t, const fq_ctx_t ctx);
fq_poly_struct* list_fq_poly_get(const list_fq_poly_t, const ulong);

#endif