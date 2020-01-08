// Interface pour la spécification du module Glosaries dans le cas d'une
//    structure de données représentant un ensemble de lexiques.

#ifndef GLOSARIES__H
#define GLOSARIES__H

#include <stdbool.h>
#include <stdio.h>
#include "word.h"

// struct glosaries, glosaries:

typedef struct glosaries glosaries;

//  Les fonctions qui suivent ont un comportement indéterminé si leurs
//    paramètres de type glosaries * n'est pas l'adresse d'un objet
//    préalablement renvoyé par glosaries_empty et non révoqué depuis par
//    glosaries_dispose.

// glosaries_empty: crée une structure de donnés correspondant à un glosaries
// vide et renvois son pointeur associé ou NULL en cas de dépassement de
// capacitée
extern glosaries *glosaries_empty(void);

//  glosaries__add_glosary_to_word: permet l'ajout du lexique gl a un mot créé
//    à partir de str si il n'existe pas déjà, la fonction gère elle même la
//    mémoire uttilisée pour la création du lexique et du mot.
//    cette fonction a un comportement indéterminé si le lexique était déja
//    ajouté au mot
extern bool glosaries_add_glosary_to_word(glosaries *g, char *str, char *gl);

// glosaries_display: retourne les informations lié à notre
//    glosaries tel que sa présence dans les différents lexiques
//    ainsi que son numéro de ligne dans notre texte.
extern bool glosaries_display(glosaries *g, char *outputFileName);

// glosaries_load_file: charge un fichier lexique. Si notre paramètre de type
//    FILE* est NULL alors la fonction renverra false sinon renvoie true.
extern bool glosaries_load_glosary_file(glosaries *g, char *fileName,
    bool lowerCase, bool upperCase);

extern bool glosaries_read_text(glosaries *g, char *fileName, bool lowerCase,
    bool upperCase);

// glosaries_dispose: libère toutes les ressources associés à la structure
//    de *g puis affecte à *g la valeur NULL
extern void glosaries_dispose(glosaries **g);

#endif
