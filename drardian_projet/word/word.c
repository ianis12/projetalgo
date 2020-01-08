//  Implantation pour la spécification du TDA mot dans le cas d'une structure de
//    données représantant un mot ainsi que les lexiques dans lesquelles on le
//    trouve et aussi les lignes sur lesquelles il apparait dans un hypotétique
//    texte non spécifié ici.

#include <stdlib.h>
#include "list.h"
#include "word.h"
#include <stdio.h>

//  struct word, word: structure permettant de gérer un mot
//  L'utilisateur doit garantir que l'adresse de chaque objet fournie aux
//    diverses fonctions est valide durant toute la durée de la présence de
//    l'objet dans le mot. Si des opérations d'allocation dynamique
//    sont effectuées, elles le sont pour la gestion propre de la structure de
//    données, et en aucun cas pour réaliser de copies ou des destructions
//    d'objets

typedef struct word word;

struct word {
  list *lines;
  list *glosaries;
};

word *word_create(void) {
  word *w = malloc(sizeof(word));
  if (w == NULL) {
    return NULL;
  }
  w->lines = list_empty();
  if (w->lines == NULL) {
    free(w);
    return NULL;
  }
  w->glosaries = list_empty();
  if (w->glosaries == NULL) {
    list_dispose(&w->lines);
    free(w);
    return NULL;
  }
  return w;
}

bool word_add_line(word *w, size_t line) {
  size_t *l = malloc(sizeof(size_t));
  if (l == NULL) {
    return false;
  }
  *l = line;
  if (list_put(w->lines, l) == NULL) {
    free(l);
    return false;
  }
  return true;
}

bool word_add_glosary(word *w, const char *glosary) {
  if (list_put(w->glosaries, glosary) == NULL) {
    return false;
  }
  return true;
}

size_t word_next_line(word *w) {
  return *((size_t *) list_next(w->lines));
}

bool word_can_next_line(word *w) {
  return list_can_next(w->lines);
}

void word_reset_current_line(word *w) {
  list_reset_current(w->lines);
}

char *word_next_glosary(word *w) {
  return (char *) list_next(w->glosaries);
}

bool word_can_next_glosary(word *w) {
  return list_can_next(w->glosaries);
}

void word_reset_current_glosary(word *w) {
  list_reset_current(w->glosaries);
}

void word_dispose(word **w) {
  list_reset_current((*w)->lines);
  while (list_can_next((*w)->lines)) {
    size_t *ptr = (size_t *) list_next((*w)->lines);
    free(ptr);
  }
  list_dispose(&(*w)->glosaries);
  list_dispose(&(*w)->lines);
  free(*w);
  *w = NULL;
}
