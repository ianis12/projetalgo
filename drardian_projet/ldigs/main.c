#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "ldigs.h"

//  stop : lit des caractères sur l'entrée standard jusqu'à détecter la fin de
//    l'entrée ou obtenir 'q', 'Q' ou '\n'. Renvoie zéro si '\n' est obtenu, une
//    valeur non nulle sinon
int stop(void);

//ldigs_move_all_head : déplace en tête de la liste associée à dest la liste
// associée à src. En cas de succès, la liste associée à src est vide à la
// terminaison et la fonction renvoie zéro. En cas d’échec, en particulier
//parce que src == dest, renvoie une valeur non nulle
int ldigs_move_all_head(ldigs *src, ldigs *dest);

// ldigs_partition_pivot : déplace respectivement dans les listes associées à
// slth, seq et sgth, les éléments de la liste associée à s qui sont
// strictement inférieurs, égaux et strictement supérieurs au premier élément
// de la liste associée à s. Si la liste associée à s est vide, les quatre
// listes demeurent inchangées. À la terminaison, la liste associée à s est
// vide
void ldigs_partition_pivot(ldigs *s, ldigs *slth, ldigs *seq, ldigs *sgth);


//ldigs_quicksort : recompose la liste associée à s selon la méthode du tri
//rapide. Renvoie zéro en cas de succès, une valeur non nulle en cas d’échec
int ldigs_quicksort(ldigs *s);


int main(void) {
  printf("--- Trials on ldigs module\n"
      "--- Type ctrl+d or enter 'q' or 'Q' to exit\n\n");
  srand(0);
  ldigs *s;
  ldigs *s1;
  while (1) {
    s = ldigs_empty();
    s1 = ldigs_empty();

    for (int k = 0; k < 15; ++k) {
      if (k > 0) {
        printf(" ");
      }
      digs ds;
      digs_rand(&ds);
      digs_fput(&ds, stdout);
      ldigs_insert_head(s, &ds);
    }
    printf("\n-----Execution des fonctions :\n");

    ldigs_fput(s, stdout);
    printf("\n-----Execution de ldigs_move_head_head(s,s1)\n");
    printf("\ns :");
    ldigs_move_head_head(s, s1);
    ldigs_fput(s, stdout);
    printf("\ns1 : ");
    ldigs_fput(s1, stdout);
    printf("\n-----Execution de ldigs_move_all_head(s,s1)\n");
    ldigs_move_all_head(s, s1);
    printf("\ns : ");
    ldigs_fput(s, stdout);
    printf("\ns1 : ");
    ldigs_fput(s1, stdout);
    printf("\n-----Execution de ldigs_quicksort(s1)\n");
    ldigs_quicksort(s1);
    printf("\ns1 : ");
    ldigs_fput(s1, stdout);
    
    printf("\n-----Execution de ldigs_dispose pour s et s1\n");
    ldigs_dispose(&s);
    ldigs_dispose(&s1);
    printf("\n> ");
    if (stop()) {
      return EXIT_SUCCESS;
    }
  }
}

int stop(void) {
  while (1) {
    int c = getchar();
    if (c == EOF || toupper(c) == 'Q') {
      return 1;
    }
    if (c == '\n') {
      return 0;
    }
  }
}

int ldigs_move_all_head(ldigs *src, ldigs *dest) {
  if (ldigs_is_empty(src) || src == dest) {
    return EXIT_FAILURE;
  }
  ldigs *p = ldigs_empty();
  while (!ldigs_is_empty(src)) {
    ldigs_move_head_head(src, p);
  }
  while (!ldigs_is_empty(p)) {
    ldigs_move_head_head(p, dest);
  }
  ldigs_dispose(&p);
  return EXIT_SUCCESS;
}

void ldigs_partition_pivot(ldigs *s, ldigs *slth, ldigs *seq, ldigs *sgth) {
  digs v1;
  digs v2;
  ldigs_head_value(s, &v1);
  ldigs_move_head_head(s, seq);
  while (!ldigs_is_empty(s)) {
    ldigs_head_value(s, &v2);
    if (digs_compar(&v1, &v2) == 0) {
      ldigs_move_head_head(s, seq);
    }
    if (digs_compar(&v1, &v2) > 0) {
      ldigs_move_head_head(s, slth);
    }
    if (digs_compar(&v1, &v2) < 0) {
      ldigs_move_head_head(s, sgth);
    }
  }
}

int ldigs_quicksort(ldigs *s) {
  if (ldigs_is_empty(s)) {
    return EXIT_FAILURE;
  }
  ldigs *p = ldigs_empty();
  ldigs_move_all_head(s, p);
  ldigs *inf = ldigs_empty();
  ldigs *eq = ldigs_empty();
  ldigs *sup = ldigs_empty();
  while (!ldigs_is_empty(p)) {
    ldigs_partition_pivot(p, inf, eq, sup);
  }
  if (ldigs_is_empty(p)) {
    ldigs_move_all_head(sup, s);
    ldigs_move_all_head(eq, s);
    ldigs_move_all_head(inf, s);
  }
  free(p);
  return EXIT_SUCCESS;
}
