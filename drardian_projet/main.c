#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
//#include "hashtable.h"
//#include "holdall.h"
#include "list.h"
#include "option.h"
//#include "word.h"
//#include "glosaries.h"

#define WORD_LENGTH_MAX 63

#define PRINT_MSG(format, ...)  \
  printf("--- " format "\n", __VA_ARGS__)

#define PRINT__STDERR(class, format, ...) \
  fprintf(stderr, "*** " class ": " format "\n", __VA_ARGS__)

#define PRINT_ERR(format, ...)  PRINT__STDERR("Error", format, __VA_ARGS__)
#define PRINT_WRN(format, ...)  PRINT__STDERR("Warning", format, __VA_ARGS__)

//void fileTreatment(FILE *tabFileInput, FILE *tabFileOutput);

int main(int argc, char **argv) {
  bool sort;
  bool lowerCase;
  bool upperCase;
  bool asIsCase;
  char *inputFileName;
  char *outputFileName;
  list *glosariesFileNamesList;
  glosariesFileNamesList = list_empty();
  option(argc, argv, &sort, &lowerCase, &upperCase, &asIsCase, &inputFileName, 
      &outputFileName, glosariesFileNamesList);
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
