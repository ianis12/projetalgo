// Interface pour la spécification du module Glosaries dans le cas d'une
//    structure de données représentant un ensemble de lexiques.

#ifndef GLOSARIES__H
#define GLOSARIES__H

#include <stdbool.h>

// struct glosaries, glosaries: 

typedef struct glosaries glosaries;

//  Les fonctions qui suivent ont un comportement indéterminé si leurs 
//    paramètres de type glosaries * n'est pas l'adresse d'un objet  
//    préalablement renvoyé par glosaries_empty et non révoqué depuis par
//    glosaries_dispose.

// list_empty: crée une structure de donnés correspondant à une liste vide et
//    renvois son pointeur associé ou NULL en cas de dépassement de capacitée
extern glosaries glosaries_empty();

// list_put: insère l'élément xptr en queue de la liste associée à l, renvoies
//    NULL si xptr vaut NULL ou en cas de dépassement de capacitée, renvois 
//    xptr sinon
extern const void *glosaries_add(glosaries *g, word *w, const void *xptr);

// glosaries_informations: retourne les informations lié à notre 
//    glosaries tel que sa présence dans les différents lexiques
//    ainsi que son numéro de ligne dans notre texte.    
extern char* glosaries_informations(glosaries *g);

// glosaries_load_file: charge un fichier lexique. Si notre paramètre de type
//    FILE* est NULL alors la fonction renverra false sinon renvoie true.
extern bool glosaries_load_file(glosaries *g, FILE *f);

// glosaries_dispose: libère toutes les ressources associés à la structure 
//    de *g puis affecte à *g la valeur NULL
extern void glosaries_dispose(glosaries **g);


#endif
