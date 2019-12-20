#include <stdbool.h>
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

//  Les fonctions qui suivent ont un comportement indéterminé si leur paramètre
//    de type list * n'est pas l'adresse d'un objet préalablement renvoyé
//    par list_empty et non révoqué depuis par list_dispose.

// list_empty: crée une structure de donnés correspondant à une liste vide et
//    renvois son pointeur associé ou NULL en cas de dépassement de capacitée
list *list_empty() {
  list *l = malloc(sizeof(list));
  if (l == NULL) {
    return NULL;
  }
  l->tail = NULL;
  l->size = 0;
  l->current = NULL;
  return l;
}

// list_put: insère l'élément xptr en queue de la liste associée à l, renvoies
//    NULL si xptr vaut NULL ou en cas de dépassement de capacitée, renvois xptr
//    sinon
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

// list_next: permet de faire avancer le pointeur courant de la liste associée à
//    l vers la valeure suivante au sens de la liste. Elle renvois l'élément
//    courant par la même occasion.
//    Cette fonction a un comportement indéterminé si list_can_next n'est pas
//    vérifié en entrée.
//    Si l'élément courant est le dernier élément de la liste, list_next le
//    renvois et list_can_next renverra false à ses prochains appels, pour
//    sortir de cet cet état, il faut alors uttiliser list_reset_current pour
//    remetre le pointeur courant sur le premier élément de la liste
const void *list_next(list *l) {
  cell *temp = l->current;
  l->current = l->current->next;
  return temp->valueptr;
}

// list_can_next: renvoies un booléen représentant la capacitée de la liste
//    associée à l, d'avancer le pointeur courant vers le prochain élément,
//    c'est à dire : revoies true si cet élément existe ou si l'élément courant
//    est le dernier élément, false sinon renvois donc false en cas de liste
//    vide
bool list_can_next(const list *l) {
  return !(l->current == NULL);
}

// list_reset_current: met le pointeur courant de la liste associée à l au début
//    de celle ci. Le prochain appel à list_next le renverra (donc sauf si la
//    liste est vide).
void list_reset_current(list *l) {
  l->current = l->head;
}

// list_is_empty: retourne un booléen représentant l'état de vacuité de la liste
//    associée a l, true si la liste est vide, false sinon
bool list_is_empty(const list *l) {
  return l->size;
}

// list_size: retourne le nombre d'éléments présent dans la liste associée a l
size_t list_size(const list *l) {
  return l->size;
}

// list_dispose: libère toutes les ressources associés à la structure de *l puis
//    affecte à *l la valeure NULL
//void list_dispose(list **l) {

//}
