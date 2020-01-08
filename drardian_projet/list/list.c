//  Implantation polymorphe pour la spécification du TDA Liste(T) dans le cas
//    d'une liste simplement chainée avec insertion en queue et avec
//    spécification d'une méthode simple d'itération efficace sur ses éléments
// dans
//    l'ordre de la liste, grâce à un pointeur d'élément courant qui peut soit
//    avancer (grâce à la fonction list_next) ou revenir au début de la liste
//    (grâce à la fonction list_reset_current).

#include <stdlib.h>
#include "list.h"

// struc list, list: structure permettant de gérer une liste dont les valeures
//    sont des objets quelconques.
//  La structure de données ne stocke pas les objets mais des pointeurs vers ces
//    objets. Les pointeurs sont du type générique const void *
//  L'utilisateur doit garantir que l'adresse de chaque objet fournie aux
//    diverses fonctions est valide durant toute la durée de la présence de
//    l'objet dans la liste. Si des opérations d'allocation dynamique
//    sont effectuées, elles le sont pour la gestion propre de la structure de
//    données, et en aucun cas pour réaliser de copies ou des destructions
//    d'objets
//  Dans le cadre de ce projet, l'opération de suppression d'éléments étant
//    inutile, elle n'est pas définie.

typedef struct cell cell;

struct cell {
  const void *valueptr;
  cell *next;
};

struct list {
  cell *tail;
  cell *head;
  size_t size;
  cell *current;
};

list *list_empty(void) {
  list *l = malloc(sizeof(list));
  if (l == NULL) {
    return NULL;
  }
  l->head = NULL;
  l->tail = NULL;
  l->size = 0;
  l->current = NULL;
  return l;
}

const void *list_put(list *l, const void *xptr) {
  cell *c = malloc(sizeof(cell));
  if (c == NULL) {
    return NULL;
  }
  ++(l->size);
  c->valueptr = xptr;
  c->next = NULL;
  if (l->head == NULL) {
    l->current = c;
    l->tail = c;
    l->head = c;
    return c;
  }
  l->tail->next = c;
  l->tail = c;
  return c;
}

const void *list_next(list *l) {
  cell *temp = l->current;
  l->current = l->current->next;
  return temp->valueptr;
}

bool list_can_next(const list *l) {
  return !(l->current == NULL);
}

void list_reset_current(list *l) {
  l->current = l->head;
}

bool list_is_empty(const list *l) {
  return l->size == 0;
}

size_t list_size(const list *l) {
  return l->size;
}

void list_dispose(list **l) {
  if ((*l)->head != NULL) {
    cell *cptr = (*l)->head;
    (*l)->head = (*l)->head->next;
    free(cptr);
    list_dispose(l);
  } else {
    free(*l);
    *l = NULL;
  }
}
