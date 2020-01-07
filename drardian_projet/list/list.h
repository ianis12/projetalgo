//  Interface polymorphe pour la spécification du TDA Liste(T) dans le cas d'une
//    liste simplement chainée avec insertion en queue et avec spécification
//    d'une méthode simple d'itération efficace sur ses éléments dans l'ordre de
//    la liste, grâce à un pointeur d'élément courant qui peut soit avancer
//    (grâce à la fonction list_next) ou revenir au début de la liste
//    (grâce à la fonction list_reset_current).

#ifndef LIST__H
#define LIST__H

#include <stddef.h>
#include <stdbool.h>

//  struc list, list: structure permettant de gérer une liste dont les valeures
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

typedef struct list list;

//  Les fonctions qui suivent ont un comportement indéterminé si leur paramètre
//    de type list * n'est pas l'adresse d'un objet préalablement renvoyé
//    par list_empty et non révoqué depuis par list_dispose. Cette règle souffre
//    néamoins d'une exception: la fonction list_dispose tolère que cet argument
//    ait pour valeure NULL

//  list_empty: crée une structure de donnés correspondant à une liste vide et
//    renvois son pointeur associé ou NULL en cas de dépassement de capacitée
extern list *list_empty(void);

//  list_put: insère l'élément xptr en queue de la liste associée à l, renvoies
//    NULL si xptr vaut NULL ou en cas de dépassement de capacitée, renvois xptr
//    sinon
extern const void *list_put(list *l, const void *xptr);

//  list_next: permet de faire avancer le pointeur courant de la liste associée
//    à l vers la valeure suivante au sens de la liste. Elle renvois l'élément
//    courant par la même occasion.
//    Cette fonction a un comportement indéterminé si list_can_next n'est pas
//    vérifié en entrée.
//    Si l'élément courant est le dernier élément de la liste, list_next le
//    renvois et list_can_next renverra false à ses prochains appels, pour
//    sortir de cet cet état, il faut alors uttiliser list_reset_current pour
//    remetre le pointeur courant sur le premier élément de la liste
extern const void *list_next(list *l);

//  list_can_next: renvoies un booléen représentant la capacitée de la liste
//    associée à l, d'avancer le pointeur courant vers le prochain élément,
//    c'est à dire : revoies true si cet élément existe ou si l'élément courant
//    est le dernier élément, false sinon renvois donc false en cas de liste
//    vide
extern bool list_can_next(const list *l);

//  list_reset_current: met le pointeur courant de la liste associée à l au
//    début de celle ci. Le prochain appel à list_next le renverra (donc sauf si
//    la liste est vide).
extern void list_reset_current(list *l);

//  list_is_empty: retourne un booléen représentant l'état de vacuité de la
//    liste associée a l, true si la liste est vide, false sinon
extern bool list_is_empty(const list *l);

//  list_size: retourne le nombre d'éléments présent dans la liste associée a l
extern size_t list_size(const list *l);

//  list_dispose: libère toutes les ressources associés à la structure de *l
//    puis affecte à *l la valeure NULL
extern void list_dispose(list **l);

#endif
