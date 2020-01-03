#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../list/list.h"
#include "option.h"

#define FUN_FAILURE -1
#define FUN_SUCCESS 0

#define OPT_FILE_GLOSARY "-"

#define OPT_LONG "--"
#define OPT_LONG_HELP OPT_LONG "help"
#define OPT_LONG_SORT OPT_LONG "sort"
#define OPT_LONG_CASE OPT_LONG "case"
#define OPT_LONG_INPUT OPT_LONG "input"
#define OPT_LONG_OUTPUT OPT_LONG "output"

#define ARG_EQUAL "="

#define ARG_CASE_LOWER "lower"
#define ARG_CASE_AS_IS "as-is"
#define ARG_CASE_UPPER "upper"

#define OPT_SHORT "-"
#define OPT_SHORT_SORT OPT_SHORT "S"
#define OPT_SHORT_LOWER OPT_SHORT "l"
#define OPT_SHORT_AS_IS OPT_SHORT "s"
#define OPT_SHORT_UPPER OPT_SHORT "u"
#define OPT_SHORT_INPUT OPT_SHORT ""
#define OPT_SHORT_INPUT OPT_SHORT "i"
#define OPT_SHORT_OUPUT OPT_SHORT "o"


#define PRINT_MSG(format, ...)  \
  printf("--- " format "\n", __VA_ARGS__)

#define PRINT__STDERR(class, format, ...) \
  fprintf(stderr, "*** " class ": " format "\n", __VA_ARGS__)

#define PRINT_ERR(format, ...)  PRINT__STDERR("Error", format, __VA_ARGS__)
#define PRINT_WRN(format, ...)  PRINT__STDERR("Warning", format, __VA_ARGS__)

#define STR(s)  #s
#define XSTR(s) STR(s)

bool is_arg_correct(char * arg, char * value, int start);
bool validFile(char * filename);

int option(int argc, char** argv, bool* sort, bool* lowerCase, bool* upperCase,
    bool* asIsCase, list* inputFileNamesList, list* outputFileNamesList) {
// VALEURS PAR DEFAUT:

  *sort = false;
  *lowerCase = false;
  *upperCase = false;
  *asIsCase = false;
  inputFileNamesList = list_empty();
  outputFileNamesList = list_empty();

// OPTIONS:
  bool isWaitingForFile = false;
  bool isWaitingForInputFile = false;
  bool isWaitingForOutputFile = false;
  
  for (int k = 1; k <= argc; ++k) {
    if (isWaitingForFile) {
      if (!validFile(argv[k])) {
        return FUN_FAILURE;
      }
      list_put(inputFileNamesList, argv[k]);
    }
    if (isWaitingForInputFile) {
      if (!validFile(argv[k])) {
        return FUN_FAILURE;
      }
      list_put(inputFileNamesList, argv[k]);
    }
    if (isWaitingForOutputFile) {
      if (!validFile(argv[k])) {
        return FUN_FAILURE;
      }
      list_put(outputFileNamesList, argv[k]);
    }
    if (strcmp(argv[k], OPT_LONG_HELP) == 0) {
      help();
    }
    if (strcmp(argv[k], OPT_LONG_SORT) == 0
        || strcmp(argv[k], OPT_SHORT_SORT) == 0) {
      sort = true;
    }
    if (is_arg_correct(OPT_LONG_CASE ARG_EQUAL, argv[k], 0) {
      if (is_arg_correct(ARG_CASE_LOWER, argv[k],
          strlen(OPT_LONG_CASE ARG_EQUAL)) {
        lowerCase = true;
      }
      if (is_arg_correct(ARG_CASE_AS_IS, argv[k],
          strlen(OPT_LONG_CASE ARG_EQUAL)) {
        asIsCase = true;
      }
      if (is_arg_correct(ARG_CASE_UPPER, argv[k],
          strlen(OPT_LONG_CASE ARG_EQUAL)) {
        upperCase = true;
      }
      goto error;
    }
    if (strcmp(OPT_SHORT_LOWER, argv[k]) {
      lowerCase = true;
    }
    if (strcmp(OPT_SHORT_AS_IS, argv[k]) {
      asIsCase = true;
    }
    if (strcmp(OPT_SHORT_UPPER, argv[k]) {
      upperCase = true;
    }
    if (strcmp(OPT_FILE_GLOSARY, argv[k]) {
      isWaitingForFile = true;
    }
    if (strcmp(OPT_SHORT_INPUT OPT_SHORT, argv[k]) {
      isWaitingForInputFile = true;
    }
    if (strcmp(OPT_SHORT_OUTPUT OPT_SHORT, argv[k]) {
      isWaitingForOuputFile = true;
    }
    if (is_arg_correct(OPT_LONG_CASE ARG_EQUAL, argv[k], 0) {
      if (is_arg_correct(ARG_CASE_LOWER, argv[k],
          strlen(OPT_LONG_CASE ARG_EQUAL)) {
        caseLowerEnable = true;
      }
      if (is_arg_correct(ARG_CASE_AS_IS, argv[k],
        strlen(OPT_LONG_CASE ARG_EQUAL)) {
        caseAsIsEnable = true;
      }
      return FUN_FAILURE;
    }
  }
}

bool is_arg_correct(char * arg, char * value, int start){
  int k = start;
  while (arg[k] != '\0' && value[k] != '\0') {
    if (arg[k] != value[k]) {
      return false;
    }
    if (k == strlen(arg) - 1) {
      return true;
    }
    ++k;
  }
  return false;
}

bool validFile(char * filename) {
  FILE* f = fopen(filename, "r+");
  if (f == NULL) {
    return false;
  }
  fclose(f);
  return true;
}
