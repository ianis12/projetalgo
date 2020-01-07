#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#include "option.h"
#include "glosaries.h"

#define WORD_LENGTH_MAX 63
#define ANONYM_GL ""

#define PRINT_MSG(format, ...)  \
  printf("--- " format "\n", __VA_ARGS__)

#define PRINT__STDERR(class, format, ...) \
  fprintf(stderr, "*** " class ": " format "\n", __VA_ARGS__)

#define PRINT_ERR(format, ...)  PRINT__STDERR("Error", format, __VA_ARGS__)
#define PRINT_WRN(format, ...)  PRINT__STDERR("Warning", format, __VA_ARGS__)

#define STR(s) #s
#define XSTR(s) STR(s)

int main(int argc, char **argv) {
  //initialiser les variables
  bool error;
  bool sort;
  bool lowerCase;
  bool upperCase;
  char *inputFileName = NULL;
  char *outputFileName = NULL;
  list *glosariesFileNamesList;
  list *anonymGlosaryWordsList;
  glosariesFileNamesList = list_empty();
  if (glosariesFileNamesList == NULL) {
	goto error_capacity;
  }
  anonymGlosaryWordsList = list_empty();
  if (anonymGlosaryWordsList == NULL) {
	goto error_capacity;
  }

  //charger les options
  option(argc, argv, &sort, &lowerCase, &upperCase, &inputFileName,
      &outputFileName, glosariesFileNamesList, anonymGlosaryWordsList);

  glosaries *gl = glosaries_empty();
  if (gl == NULL) {
	  goto error_capacity;
  }

  // remplir le lexique anonyme:
  while (list_can_next(anonymGlosaryWordsList)) {
    char *str = (char *)list_next(anonymGlosaryWordsList);
    char strb[WORD_LENGTH_MAX + 1];
    strcpy(strb, str);
    if (!glosaries_add_glosary_to_word(gl, strb, ANONYM_GL)) {
	  goto error_capacity;
    }
  }
  //charger les ficher lexiques
  while (list_can_next(glosariesFileNamesList)) {
	char *fileStr = (char *)list_next(glosariesFileNamesList);
    if (!glosaries_load_glosary_file(gl, fileStr, lowerCase, upperCase)) {
      goto error_capacity;
    }
  }
  // récupérer l'entrée
  glosaries_read_text(gl, inputFileName, lowerCase, upperCase);
  // afficher les lexiques
  glosaries_display(gl, outputFileName);
  goto dispose;
error_capacity:
  error = true;
  goto _exit;
dispose:
  error = false;
_exit:
  if (anonymGlosaryWordsList != NULL) {
	list_reset_current(anonymGlosaryWordsList);
	while (list_can_next(anonymGlosaryWordsList)) {
      char *str = (char *)list_next(anonymGlosaryWordsList);
	  free(str);
	}
  }
  if (glosariesFileNamesList != NULL) {
	list_reset_current(glosariesFileNamesList);
	while (list_can_next(glosariesFileNamesList)) {
      char *str = (char *)list_next(glosariesFileNamesList);
	  free(str);
	}
  }
  free(inputFileName);
  free(outputFileName);
  glosaries_dispose(&gl);
  list_dispose(&anonymGlosaryWordsList);
  list_dispose(&glosariesFileNamesList);
  if (error) {
	  return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
