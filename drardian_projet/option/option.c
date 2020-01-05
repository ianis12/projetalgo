#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "list.h"
#include "option.h"

#define FUN_FAILURE -1
#define FUN_SUCCESS 0

#define WORD_LENGTH_MAX 63

#define OPT_SHORT "-"
#define OPT_EQUAL "="
#define OPT_LONG "--"
#define OPT_CASE "case"
#define OPT_LOWER "lower"
#define OPT_AS_IS "as-is"
#define OPT_UPPER "upper"

#define ARG_LONG_HELP OPT_LONG "help"
#define ARG_LONG_SORT OPT_LONG "sort"
#define ARG_LONG_INPUT OPT_LONG "input"
#define ARG_LONG_OUTPUT OPT_LONG "output"
#define ARG_LONG_LOWER OPT_LONG OPT_CASE OPT_LOWER
#define ARG_LONG_UPPER OPT_LONG OPT_CASE OPT_UPPER
#define ARG_LONG_AS_IS OPT_LONG OPT_CASE OPT_AS_IS

#define ARG_FILE_GLOSARY "-"

#define ARG_SHORT_SORT OPT_SHORT "S"
#define ARG_SHORT_LOWER OPT_SHORT "l"
#define ARG_SHORT_AS_IS OPT_SHORT "s"
#define ARG_SHORT_UPPER OPT_SHORT "u"
#define ARG_SHORT_INPUT OPT_SHORT "i"
#define ARG_SHORT_OUPUT OPT_SHORT "o"


#define PRINT_MSG(format, ...)  \
  printf("--- " format "\n", __VA_ARGS__)

#define PRINT__STDERR(class, format, ...) \
  fprintf(stderr, "*** " class ": " format "\n", __VA_ARGS__)

#define PRINT_ERR(format, ...)  PRINT__STDERR("Error", format, __VA_ARGS__)
#define PRINT_WRN(format, ...)  PRINT__STDERR("Warning", format, __VA_ARGS__)

#define STR(s)  #s
#define XSTR(s) STR(s)

bool checkArgFile(const char * arg, const char * value, size_t* start);
bool validFile(char * filename);
void help();

int option(int argc, char** argv, bool* sort, bool* lowerCase, bool* upperCase,
    bool* asIsCase, char **inputFileName, char **outputFileName, 
    list *glosaryFileNamesList) {
// VALEURS PAR DEFAUT:

  *sort = false;
  *lowerCase = false;
  *upperCase = false;
  *asIsCase = false;
  size_t offsetFile = 0;

// OPTIONS:
  bool isWaitingForGlosaryFile = false;
  bool isWaitingForInputFile = false;
  bool isWaitingForOutputFile = false;
  
  for (int k = 1; k < argc; ++k) {
    if (isWaitingForInputFile) {
	  isWaitingForInputFile = false;
	  if (validFile(argv[k])) {
        *inputFileName = malloc(strlen(argv[k]));
        strcpy(*inputFileName, argv[k]);
	  } else {
	    fprintf(stderr, "le fichier n'est pas valide\n");
	    return FUN_FAILURE;
	  }
	  continue;
	}
	if (isWaitingForGlosaryFile) {
	  isWaitingForGlosaryFile = false;
	  if (validFile(argv[k])) {
		char *fileName = malloc(strlen(argv[k]));
        list_put(glosaryFileNamesList, fileName);
	  } else {
	    fprintf(stderr, "le fichier n'est pas valide\n");
	    return FUN_FAILURE;
	  }
	  continue;
	}
	if (isWaitingForOutputFile) {
	  isWaitingForOutputFile = false;
	  if (validFile(argv[k])) {
		  *outputFileName = malloc(strlen(argv[k]));
		  strcpy(*outputFileName, argv[k]);
	  } else {
	    fprintf(stderr, "le fichier n'est pas valide\n");
	    return FUN_FAILURE;
	  }
	  continue;
	}
    if (strcmp(ARG_LONG_SORT, argv[k]) == 0
        || strcmp(ARG_SHORT_SORT, argv[k]) == 0) {
      *sort = true;
      continue;
    }
    if (strcmp(ARG_LONG_HELP, argv[k]) == 0) {
      help();
      continue;
    }
    if (strcmp(ARG_LONG_AS_IS, argv[k]) == 0
	    || strcmp(ARG_SHORT_AS_IS, argv[k]) == 0) {
	  *asIsCase = true;
	  continue;
	}
	if (strcmp(ARG_LONG_LOWER, argv[k]) == 0
	    || strcmp(ARG_SHORT_LOWER, argv[k]) == 0) {
	  *lowerCase = true;
	  continue;
	}
	if (strcmp(ARG_LONG_UPPER, argv[k]) == 0
	    || strcmp(ARG_SHORT_UPPER, argv[k]) == 0) {
	  *upperCase = true;
	  continue;
	}
	if (checkArgFile(argv[k], ARG_LONG_INPUT, &offsetFile)) {
	  if (validFile(argv[k] + offsetFile)) {
	    strcpy(*inputFileName, argv[k] + offsetFile);
	  } else {
	    fprintf(stderr, "le fichier n'est pas valide\n");
	    return FUN_FAILURE;
	  }
    }
    if (checkArgFile(argv[k], ARG_LONG_OUTPUT, &offsetFile)) {
	  if (validFile(argv[k] + offsetFile)) {
	    strcpy(*outputFileName, argv[k] + offsetFile);
	  } else {
	    fprintf(stderr, "le fichier n'est pas valide\n");
	    return FUN_FAILURE;
	  }
    }
    if (strcmp(ARG_SHORT_INPUT, argv[k]) == 0) {
      isWaitingForInputFile = true;
    }
    if (strcmp(ARG_SHORT_OUPUT, argv[k]) == 0) {
	  isWaitingForOutputFile = true;
	}
	if (strcmp(ARG_FILE_GLOSARY, argv[k]) == 0) {
	  isWaitingForGlosaryFile = true;
	}
  }
  return FUN_SUCCESS;
}


bool checkArgFile(const char* arg, const char * value, size_t* k){
  *k = 0;
  while (arg[*k] != '\0' && value[*k] != '\0') {
    if (arg[*k] != value[*k]) {
      return false;
    }
    if (*k == strlen(value) - 1) {
      return true;
    }
    ++(*k);
  }
  return false;
}

bool validFile(char * filename) {
  FILE* f = fopen(filename, "rw");
  if (f == NULL) {
    return false;
  }
  fclose(f);
  return true;
}

void help() {
	printf("bonjour");
}
