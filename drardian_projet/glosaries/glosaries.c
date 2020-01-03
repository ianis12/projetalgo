// Implémentation pour la spécification du module Glosaries dans le cas d'une
//    structure de données représentant un ensemble de lexiques.
// l'uttilisateur doit garantir que ... sur les char *

#include <stdlib.h>
#include <string.h>
#include "glosaries.h"
#include "../hashtable/hashtable.h"
#include "../holdall/holdall.h"

#define WORD_LENGTH_MAX 63

#define STR(s)  #s
#define XSTR(s) STR(s)


#define FUN_SUCCESS 0
#define FUN_FAILURE -1

// struct glosaries, glosaries: 

typedef struct glosaries glosaries;

struct glosaries {
  hashtable *ht;
  holdall *ha_word;
  holdall *ha_str;
  holdall *ha_gl;
};

struct hashtable_file {
  hashtable* ht;
  FILE *f;
};

static size_t str_hashfun(const char *w);
static int str__holdall_dispose_fun(char * str);
static int word__holdall_dispose_fun(word *str);
static int word_information__holdall_fun(char *str, void *result);

static bool glosaries__add_glosary_to_word(glosaries *g, char *w, char *gl);
static void *word_information__holdall_funcontext(struct hashtable_file* ht_f,
    char *str);

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

// glosaries_informations: retourne les informations lié à notre 
//    glosaries tel que sa présence dans les différents lexiques
//    ainsi que son numéro de ligne dans notre texte.    
bool glosaries_informations(glosaries *g, FILE *f) {
  struct hashtable_file ht_f;
  ht_f.ht = g->ht;
  ht_f.f = f;
  holdall_apply_context(g->ha_str, (int (*)(void *, void *))
      word_information__holdall_fun, (void *(*)(void *, void *))
      word_information__holdall_funcontext, &ht_f);
  return NULL;
}

// glosaries_load_file: charge un fichier lexique. Si notre paramètre de type
//    FILE* est NULL alors la fonction renverra false sinon renvoie true.
bool glosaries_load_file(glosaries *g, FILE *f, char *gl) {
  char w[WORD_LENGTH_MAX];
  while (fscanf(f, "%" XSTR(WORD_LENGTH_MAX) "s", w) != EOF) {
    if (!glosaries__add_glosary_to_word(g, w, gl)) {
      return false;
    }
  }
  fclose(f);
  if (ferror(f)) {
    return false;
  }
  return true;
}

// glosaries_dispose: libère toutes les ressources associés à la structure 
//    de *g puis affecte à *g la valeur NULL
void glosaries_dispose(glosaries **g) {
  holdall_apply((*g)->ha_word, (int (*)(void *)) word__holdall_dispose_fun);
  holdall_apply((*g)->ha_gl, (int (*)(void *)) str__holdall_dispose_fun);
  holdall_apply((*g)->ha_str, (int (*)(void *)) str__holdall_dispose_fun);
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

//  glosaries__add_glosary_to_word: permet l'ajout du lexique gl a un mot créé
//    à partir de str si il n'existe pas déjà, la fonction gère elle même la 
//    mémoire uttilisée pour la création du lexique et du mot. 
bool glosaries__add_glosary_to_word(glosaries *g, char *str, char *gl) {
  word *w = (word *)hashtable_value(g->ht, str);
  char *glc = malloc(strlen(gl));
  if (glc == NULL) {
	  return false;
  }
  strcpy(glc, gl);
  if (w == NULL) {
    char *strc = malloc(strlen(str));
    if (strc == NULL) {
      free(glc);
      return false;
    }
    strcpy(strc, str);
    w = word_create();
    if (w == NULL) {
      free(strc);
      free(glc);
      return false;
	}
    if (!word_add_glosary(w, glc)) {
      free(strc);
      free(glc);
      word_dispose(&w);
      return false;
	}
    if (holdall_put(g->ha_str, strc) != 0) {
      free(strc);
      free(glc);
      word_dispose(&w);
	  return false;
	}
    if (holdall_put(g->ha_word, w) != 0) {
      free(strc);
      free(glc);
	  word_dispose(&w);
      return false;
    }
    if (hashtable_add(g->ht, strc, w) == NULL) {
      free(strc);
      free(glc);
      word_dispose(&w);
      return false;
	}
	return true;
  }
  if (!word_add_glosary(w, glc)) {
    free(glc);
    return false;
  }
  return true;
}

int str__holdall_dispose_fun(char * str) {
  free(str);
  return 0;
}

int word__holdall_dispose_fun(word *str) {
  word_dispose(&str);
  return 0;
}

int word_information__holdall_fun(char *str, void *result) {
  if (str == NULL) {
	  return FUN_FAILURE;
  }
  if (result != NULL) {
    return FUN_FAILURE;
  }
  return FUN_SUCCESS;
}

void *word_information__holdall_funcontext(struct hashtable_file* ht_f,
    char *str) {
  if (fprintf(ht_f->f, "%s", str) < 0) {
	  return ht_f;
  }
  word *w = (word *)hashtable_value(ht_f->ht, str);
  while (word_can_next_glosary(w)) {
    if (fprintf(ht_f->f, "%s", "\t") < 0) {
	    return ht_f;
    }
    if (fprintf(ht_f->f, "%s", word_next_glosary(w)) < 0) {
	    return ht_f;
    }
  }
  if (fprintf(ht_f->f, "%s", "\t") < 0) {
	  return ht_f;
  }
  bool first = true;
  while (word_can_next_line(w)) {
    if (!first) {
        if (fprintf(ht_f->f, "%s", ",") < 0) {
	    return ht_f;
      }
    }
    if (fprintf(ht_f->f, "%zu", word_next_line(w)) < 0) {
	    return ht_f;
    }
    first = false;
  }
  if (fprintf(ht_f->f, "%s", "\n") < 0) {
    return ht_f;
  }
  return NULL;
}
