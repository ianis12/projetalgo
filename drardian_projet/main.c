#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
#include "option.h"
#include "glosaries.h"

#define WORD_LENGTH_MAX 63
#define ANONYM_GL "anonym"

#define PRINT_MSG(format, ...)  \
  printf("--- " format "\n", __VA_ARGS__)

#define PRINT__STDERR(class, format, ...) \
  fprintf(stderr, "*** " class ": " format "\n", __VA_ARGS__)

#define PRINT_ERR(format, ...)  PRINT__STDERR("Error", format, __VA_ARGS__)
#define PRINT_WRN(format, ...)  PRINT__STDERR("Warning", format, __VA_ARGS__)

#define STR(s)  #s
#define XSTR(s) STR(s)

//void fileTreatment(FILE *tabFileInput, FILE *tabFileOutput);

int main(int argc, char **argv) {
  bool error;
  bool sort;
  bool lowerCase;
  bool upperCase;
  bool asIsCase;
  char *inputFileName;
  char *outputFileName;
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
  option(argc, argv, &sort, &lowerCase, &upperCase, &asIsCase, &inputFileName, 
      &outputFileName, glosariesFileNamesList, anonymGlosaryWordsList);
  glosaries *gl = glosaries_empty();
  if (gl == NULL) {
	  goto error_capacity;
  }
  // remplir le lexique anonyme:
  while (list_can_next(anonymGlosaryWordsList)) {
    char *str = (char *)list_next(anonymGlosaryWordsList);
    char strb[WORD_LENGTH_MAX];
    strcpy(strb, str);
    free(str);
    glosaries_add_glosary_to_word(gl, strb, ANONYM_GL);
  }
  
  FILE *input;
  if (inputFileName != NULL) {
    input = fopen(inputFileName, "r");
    free(inputFileName);
  } else {
	input = stdin;
  }
  char strin[WORD_LENGTH_MAX];
  while (fscanf(input, "%" XSTR(WORD_LENGTH_MAX) "s", strin) != EOF) {
    glosaries_add_glosary_to_word(gl, strin, ANONYM_GL);
  }
  //charger les ficher lexiques
  while (list_can_next(glosariesFileNamesList)) {
    char *fileStr = (char *)list_next(glosariesFileNamesList);
    FILE *f = fopen(fileStr, "r");
    glosaries_load_file(gl, f, fileStr);
    free(fileStr);
  }
  
  //FILE *output;
  if (outputFileName == NULL) {
  //  output = stdout;
  } else {
  //  output = fopen(outputFileName, "w");
    printf("free str\n");
    free(outputFileName);
  }
  
  //glosaries_display(gl, output);
  
  goto dispose;
error_capacity:
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
  error = true;
  goto _exit;
dispose:
  error = false;
_exit:
  glosaries_dispose(&gl);
  list_dispose(&anonymGlosaryWordsList);
  list_dispose(&glosariesFileNamesList);
  if (error) {
	  return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
/*
void fileTreatment(glosary *gl, FILE *tabFileOutput) {
  if (!list_is_empty(inputFileNamesList)) {
    size_t id = 0;
    while (list_can_next(inputFileNamesList)) {
      file = *((char **)list_next(inputFileNamesList));
      FILE *inputFile = fopen(file, "r");
      if (inputFile == NULL) {
        return NULL;
      }
      glosaries_load_file(inputFile);
      fclose(inputFile);
      ++id;
    }
    list_reset_current(inputFileNamesListFileNamesList);
  }
  id = 0;
  if (!list_is_empty(outputFileNamesList)) {
    while (list_can_next(outputFileNamesList)) {
      file = *((char **)list_next(outputFileNamesList));
      tabFileOutput[id] = fopen(file, "r");
      ++id;
    }
    list_reset_current(outputFileNamesList);
  }
}
*/
