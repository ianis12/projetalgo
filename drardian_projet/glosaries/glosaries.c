// Implémentation pour la spécification du module Glosaries dans le cas d'une
//    structure de données représentant un ensemble de lexiques.
// l'uttilisateur doit garantir que ... sur les char *

#include <stdlib.h>
#include <string.h>
#include "glosaries.h"
#include "hashtable.h"
#include "holdall.h"
#include "list.h"

#define WORD_LENGTH_MAX 63

#define STR(s)  #s
#define XSTR(s) STR(s)


#define FUN_SUCCESS 0
#define FUN_FAILURE -1
#define FUN_FOUND 1

// struct glosaries, glosaries: 

typedef struct glosaries glosaries;

struct glosaries {
  hashtable *ht;
  holdall *ha_word;
  holdall *ha_str;
  holdall *ha_gl;
  list *l_gl;
};

struct hashtable_file_list {
  list *l;
  hashtable* ht;
  FILE *f;
};

struct word_file_list {
  list *l;
  const word* w;
  FILE *f;
};

static size_t str_hashfun(const char *w);
static int str__holdall_dispose_fun(char * str);
static int word__holdall_dispose_fun(word *str);

static struct word_file_list *hashtable_file_value(
    struct hashtable_file_list *ht_f, char *str);
static int word_file_display(char *cptr, struct word_file_list *w_f);

//  Les fonctions qui suivent ont un comportement indéterminé si leurs 
//    paramètres de type glosaries * n'est pas l'adresse d'un objet  
//    préalablement renvoyé par glosaries_empty et non révoqué depuis par
//    glosaries_dispose.

// list_empty: crée une structure de donnés correspondant à une liste vide et
//    renvois son pointeur associé ou NULL en cas de dépassement de capacitée
glosaries *glosaries_empty(void) {
  glosaries *g = malloc(sizeof(glosaries));
  if (g == NULL) {
	  return NULL;
  }
  g->ht = hashtable_empty((int (*)(const void *, const void *))strcmp, 
      (size_t (*)(const void *))str_hashfun);
  if (g->ht == NULL) {
    free(g);
    return NULL;
  }
  g->ha_word = holdall_empty();
  if (g->ha_word == NULL) {
    hashtable_dispose(&g->ht);
    free(g);
    return NULL;
  }
  g->ha_str = holdall_empty();
  if (g->ha_str == NULL) {
    hashtable_dispose(&g->ht);
    holdall_dispose(&g->ha_word);
    free(g);
    return NULL;
  }
  g->ha_gl = holdall_empty();
  if (g->ha_gl == NULL) {
    holdall_dispose(&g->ha_str);
    hashtable_dispose(&g->ht);
    holdall_dispose(&g->ha_word);
    free(g);
    return NULL;
  }
  g->l_gl = list_empty();
  if (g->l_gl == NULL) {
    holdall_dispose(&g->ha_gl);
    holdall_dispose(&g->ha_str);
    hashtable_dispose(&g->ht);
    holdall_dispose(&g->ha_word);
    free(g);
    return NULL;
  }
  return g;
}

// list_put: insère l'élément xptr en queue de la liste associée à l, renvoies
//    NULL si xptr vaut NULL ou en cas de dépassement de capacitée, renvois 
//    xptr sinon
bool glosaries_add(glosaries *g, const char *str, size_t line) {
  word *w = (word *)hashtable_value(g->ht, str);
  if (w != NULL) {
    if (!word_add_line(w, line)) {
		return false;
	}
  }
  return true;
}

// glosaries_display: retourne les informations lié à notre 
//    glosaries tel que sa présence dans les différents lexiques
//    ainsi que son numéro de ligne dans notre texte.    
bool glosaries_display(glosaries *g, FILE *f) {
  fprintf(f, "\t\t");
  while (list_can_next(g->l_gl)) {
    fprintf(f, "%s", (char *) list_next(g->l_gl));
    fprintf(f, "\t");
  }
  fprintf(f, "\n");
  struct hashtable_file_list ht_f_l;
  ht_f_l.ht = g->ht;
  ht_f_l.f = f;
  ht_f_l.l = g->l_gl;
  holdall_apply_context(g->ha_str, 
      (int (*)(void *, void *)) word_file_display, 
      (void *(*)(void *, void *)) hashtable_file_value, &ht_f_l);
  return NULL;
}

// glosaries_load_file: charge un fichier lexique. Si notre paramètre de type
//    FILE* est NULL alors la fonction renverra false sinon renvoie true.
bool glosaries_load_file(glosaries *g, FILE *f, char *gl) {
  char w[WORD_LENGTH_MAX];
  while (fscanf(f, "%" XSTR(WORD_LENGTH_MAX) "s", w) != EOF) {
    if (!glosaries_add_glosary_to_word(g, w, gl)) {
      return false;
    }
  }
  if (ferror(f)) {
    fclose(f);
    return false;
  }
  fclose(f);
  return true;
}

// glosaries_dispose: libère toutes les ressources associés à la structure 
//    de *g puis affecte à *g la valeur NULL
void glosaries_dispose(glosaries **g) {
  hashtable_dispose(&(*g)->ht);
  holdall_apply((*g)->ha_word, (int (*)(void *)) word__holdall_dispose_fun);
  holdall_apply((*g)->ha_gl, (int (*)(void *)) str__holdall_dispose_fun);
  holdall_apply((*g)->ha_str, (int (*)(void *)) str__holdall_dispose_fun);
  list_dispose(&(*g)->l_gl);
  holdall_dispose(&(*g)->ha_word);
  holdall_dispose(&(*g)->ha_gl);
  holdall_dispose(&(*g)->ha_str);
  free(*g);
  *g = NULL;
}

//  OUTILS :

//  str_hashfun: foncton de hash sur le type char *
size_t str_hashfun(const char *s) {
  size_t h = 0;
  for (const unsigned char *p = (const unsigned char *) s; *p != '\0'; ++p) {
    h = 37 * h + *p;
  }
  return h;
}

char *glosaries_add_glosary(glosaries *g, char *gl) {
  bool exist = false;
  char *result = NULL;
  while (list_can_next(g->l_gl)) {
    result = (char *) list_next(g->l_gl);
    if (strcmp(gl, result) == 0) {
      exist = true;  
    }
  }
  list_reset_current(g->l_gl);
  if (!exist) {
    char *str = malloc(strlen(gl));
    if (str == NULL) {
      return NULL;
    }
    strcpy(str, gl);
    list_put(g->l_gl, str);
    holdall_put(g->ha_gl, str);
    return str;
  }
  return result;
}



//  glosaries__add_glosary_to_word: permet l'ajout du lexique gl a un mot créé
//    à partir de str si il n'existe pas déjà, la fonction gère elle même la 
//    mémoire uttilisée pour la création du lexique et du mot. 
//    cette fonction a un comportement indéterminé si le lexique était déja
//    ajouté au mot
bool glosaries_add_glosary_to_word(glosaries *g, char *str, char *gl) {
  word *w = (word *) hashtable_value(g->ht, str);
  
  if (w == NULL) {
    w = word_create();
    if (w == NULL) {
      return false;
    }
    holdall_put(g->ha_word, w);
    char *strb = malloc(strlen(str));
    if (strb == NULL) {
	  return false;
	}
	holdall_put(g->ha_str, strb);
    strcpy(strb, str);
    hashtable_add(g->ht, strb, w);
  }
  char *glptr = glosaries_add_glosary(g, gl);
  if (glptr == NULL) {
	  return false;
  }
  if (!word_add_glosary(w, glptr)) {
    return false;
  }
  return true;
}

int str__holdall_dispose_fun(char *str) {
  free(str);
  return 0;
}

int word__holdall_dispose_fun(word *str) {
  word_dispose(&str);
  return 0;
}

struct word_file_list *hashtable_file_value(struct hashtable_file_list *ht_f_l,
      char *keyptr) {
	hashtable *h = ht_f_l->ht;
	FILE *f = ht_f_l->f;
	list *l = ht_f_l->l;
	struct word_file_list *w_f_l = malloc(sizeof(struct word_file_list));
	if (w_f_l == NULL) {
		return NULL;
	}
	w_f_l->w = hashtable_value(h, keyptr);
	w_f_l->f = f;
	w_f_l->l = l;
	return w_f_l;
}

int word_file_display(char *cptr, struct word_file_list *w_f_l) {
  if (w_f_l == NULL) {
    return FUN_FAILURE;
  }
  word *w = (word *) w_f_l->w;
  FILE *f = w_f_l->f;
  list *l = w_f_l->l;
  free(w_f_l);
  fprintf(f, "%s", cptr);
  while(word_can_next_glosary(w)) {
    char *wtmp = word_next_glosary(w);
    while (wtmp != list_next(l)) {
      fprintf(f, "\t");
	}
    fprintf(f, "x");
    fprintf(f, "\t");
  }
  word_reset_current_glosary(w);
  bool can_next = word_can_next_line(w);
  while (can_next) {
    fprintf(f, "%s", (char *) word_next_line(w));
    can_next = word_can_next_line(w);
    if (can_next) {
	  fprintf(f, ",");
	}
  }
  fprintf(f, "\n");
  return FUN_SUCCESS;
}
