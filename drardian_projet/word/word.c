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

//  Les fonctions qui suivent ont un comportement indéterminié si l'adresse de
//    leur paramètre de type word * n'est pas l'adresse d'un objet préalablement
//    renvoyé par la fonction word_create et non révoqué depuis par la fonction
//    word_dispose
//  Les lignes et lexiques dans lesquelles apparait le mot sont gérés par des
//    liste provenant du module list dont l'inteface est défini dans le fichier
//    list.h, aussi l'uttilisation des fonctions suivantes peuvent être déduites
//    prèsque immédiatement de l'inteface list.h à ceci près que ces fonction
//    ont été ôté de leur caractère polymorphe pour une uttilisation simplifiée:
//    word_next_line, word_can_next_line, word_reset_current_line,
//    word_next_glosary, word_can_next_glosary, word_reset_current_glosary

//  word_create: crée une structure de donnée correspondant un mot et renvois
//    son adresse ou NULL si il y a eu dépassement de capacitée
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

//  word_add_line: ajoute aux lignes sur lesquelles on peut trouver le mot w
//    dans le texte, la ligne line et renvois true en cas de succès, false en
//    cas de dépassement de capacité
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

//  word_add_glosary: ajoute aux lexiques dans lesquels le mot w apparait,
//    le lexique représenté par la chaine de caratère glosary et renvois true
//    en cas de succès, false en cas de débordement de capacité
//  Il est bon à noter qu'aucun test n'est fait sur la chaine de caractère
//    glosary, aussi est il possible d'ajouter NULL au mot word. Cela étant
//    due aux module list uttilisé, et souhaitant une performance optimale,
//    dans ce cas-ci, la fonction renverra false et il ne sera pas possible de
//    différencier un débordement de capacitée d'une execution normale autrement
//    qu'en vérifiant manuellement la fin des lexiques a l'aide des fonctions
//    word_next_line et word_can_next_line, pour vérifier que NULL y est bien
//    présent. Pour cette raison, il est fortement déconseillé de passer NULL
//    comme valeure associée à la chaine de caractère glosary
bool word_add_glosary(word *w, const char *glosary) {
  if (list_put(w->glosaries, glosary) == NULL) {
    return false;
  }
  return true;
}

//  word_next_line: renvois la ligne courante de w et passe à la suivante.
//    Cette fonction doit être éxécutée seulement si word_can_next_line(w) a
//    renvoyé true après tout autre appel à word_next_line(w) antérieur à
//    l'appel actuel de la fonction, ou si pour une quelquonque raison
//    vous avez une preuve que l'objet courant existe bel et bien.
//    Pour plus de clartée sur cette fonction, se référer à la fonction
//    list_next du module list comme spécifié dans l'entête de ce fichier
size_t word_next_line(word *w) {
  return *((size_t *)list_next(w->lines));
}

//  word_can_next_line: indique si l'uttilisation de word_next_line(w) est
//    autorisé sur w, si l'uttilisation de word_next_line(w) n'est pas autorisé,
//    il est garanti que la cause de cette interdiction ne peut être que
//    parceque la ligne courante est la dernière dans w
//    Pour plus de clartée sur cette fonction, se référer à la fonction
//    list_can_next du module list comme spécifié dans l'entête de ce fichier.
bool word_can_next_line(word *w) {
  return list_can_next(w->lines);
}

//  word_rest_current_line: remet la ligne courante sur la première ligne
//    Pour plus de clartée sur cette fonction, se référer à la fonction
//    list_reset_current du module list comme spécifié dans l'entête de ce
//    fichier.
void word_reset_current_line(word *w) {
  list_reset_current(w->lines);
}

//  word_next_glosary: comportement comparable à word_next_line avec pour objet
//    les lexiques en lieu et place des lignes et la fonction
//    word_can_next_glosary(w) en remplacement à word_can_next_line(w)
//    Pour plus de clartée sur cette fonction, se référer à la fonction
//    list_next du module list comme spécifié dans l'entête de ce fichier.
char *word_next_glosary(word *w) {
  return (char*)list_next(w->glosaries);
}

//  word_can_next_glosary: comportement comparable à word_can_next_line avec
//    pour objet les lexiques en lieu et place des lignes et la fonction
//    word_next_glosary(w) en remplacement à word_next_line(w)
//    Pour plus de clartée sur cette fonction, se référer à la fonction
//    list_can_next du module list comme spécifié dans l'entête de ce fichier.
bool word_can_next_glosary(word *w) {
  return list_can_next(w->glosaries);
}

//  word_rest_current_glosary: remet la ligne courante sur la premièr lexique
//    Pour plus de clartée sur cette fonction, se référer à la fonction
//    list_reset_current du module list comme spécifié dans l'entête de ce
//    fichier.
void word_reset_current_glosary(word *w) {
  list_reset_current(w->glosaries);
}

//  word_dispose: libère toutes les ressources associés à la structure de *w
//    puis affecte à *w la valeur NULL
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
