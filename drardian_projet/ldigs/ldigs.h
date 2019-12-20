#ifndef LDIGS__H
#define LDIGS__H

#include <stdbool.h>
#include "digs.h"

//  struct ldigs, ldigs : structure capable de gérer une liste dynamique
//    simplement chainée sur le type digs, d'alias ldigs
typedef struct ldigs ldigs;

//  ldigs_empty : crée un objet de type ldigs. La liste est initialement vide.
//    Renvoie un pointeur vers l'objet en cas de succès, NULL en cas d'échec
extern ldigs *ldigs_empty(void);

//  ldigs_is_empty : renvoie true ou false selon que la liste associée à s est
//    vide ou non
extern bool ldigs_is_empty(const ldigs *s);

//  ldigs_head_value : si la liste associée à s n'est pas vide, affecte à *ptr
//    la valeur de l'élément de tête et renvoie zéro. Renvoie une valeur non
//    nulle sinon
extern int ldigs_head_value(const ldigs *s, digs *ptr);

//  ldigs_insert_head : insère une copie de *ptr en tête de la liste associée à
//    s. Renvoie zéro en cas de succès, une valeur non nulle en cas d'échec
extern int ldigs_insert_head(ldigs *s, digs *ptr);

//  ldigs_move_head_head : déplace l'élément de tête de la liste associée à src
//    vers la tête de la liste associée à dest. Renvoie zéro en cas de succès,
//    une valeur non nulle en cas d'échec pour cause de liste associée à src
//    vide
extern int ldigs_move_head_head(ldigs *src, ldigs *dest);

//  ldigs_fput : écrit sur le flot contrôlé par l'objet pointé par stream les
//    éléments de la liste associée à s. Les éléments sont écrits séparés par
//    une espace. Le délimiteur gauche est vide, le délimiteur droit est la fin
//    de ligne. Renvoie zéro en cas de succès, une valeur non nulle en cas
//    d'échec
extern int ldigs_fput(const ldigs *s, FILE *stream);

//  ldigs_dispose : si *sptr ne vaut pas NULL, libère les ressources allouées à
//    *sptr ainsi qu'à la liste associée puis affecte la valeur NULL à *sptr
extern void ldigs_dispose(ldigs **sptr);

#endif
