#ifndef DIGS__H
#define DIGS__H

#include <stdio.h>

#define DIGS_DEFAULT_LEN  4

#ifndef DIGS_LEN
#define DIGS_LEN DIGS_DEFAULT_LEN
#else
#if DIGS_LEN <= 0
#error illegal value of DIGS_LEN
#endif
#endif

#ifndef DIGS_COMPAR_LEN
#define DIGS_COMPAR_LEN  DIGS_LEN
#else
#if DIGS_COMPAR_LEN <= 0 || DIGS_COMPAR_LEN > DIGS_LEN
#error illegal value of DIGS_COMPAR_LEN
#endif
#endif

//  digs est le nom d'une structure capable de mémoriser une suite de DIGS_LEN
//    chiffres
typedef struct {
  char reserved__usage[DIGS_LEN];
} digs;

//  digs_fget : lit une suite de DIGS_LEN chiffres sur le flot contrôlé par
//    l'objet pointé par stream. En cas de succès, affecte cette suite à l'objet
//    pointé par ptr et renvoie zéro. En cas d'échec, renvoie une valeur non
//    nulle sans avoir modifié l'objet pointé par ptr
extern int digs_fget(digs *ptr, FILE *stream);

//  digs_fput : écrit sur le flot contrôlé par l'objet pointé par stream les
//    DIGS_LEN chiffres de la suite mémorisée dans l'objet pointé par ptr.
//    Renvoie zéro en cas de succès, une valeur non nulle en cas d'échec
extern int digs_fput(const digs *ptr, FILE *stream);

//  digs_rand : affecte une suite de DIGS_LEN chiffres pseudo-aléatoires à
//    l'objet pointé par ptr
extern void digs_rand(digs *ptr);

//  digs_compar : compare les objets pointés par ptr1 et ptr2 selon l'ordre
//    lexicographique en ne considérant que leurs préfixes de longueur
//    DIGS_COMPAR_LEN. Renvoie une valeur strictement inférieure, égale ou
//    strictement supérieure à zéro selon que le premier objet est strictement
//    inférieur, égal ou strictement supérieur au second
extern int digs_compar(const digs *ptr1, const digs *ptr2);

#endif
