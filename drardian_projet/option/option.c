#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
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

static bool checkArgFile(const char *arg, const char *value, size_t *start);
static bool validFileRead(char *filename);
static bool validFileWrite(char *filename);
static void addCorrectsWords(list *anonymGlosaryWords, char *arg);
void help();

int option(int argc, char **argv, bool *sort, bool *lowerCase, bool *upperCase,
    char **inputFileName, char **outputFileName,
    list *glosaryFileNamesList, list *anonymGlosaryWords) {
  // VALEURS PAR DEFAUT:
  *sort = false;
  *lowerCase = false;
  *upperCase = false;
  size_t offsetFile = 0;
  // OPTIONS:
  bool isWaitingForGlosaryFile = false;
  bool isWaitingForInputFile = false;
  bool isWaitingForOutputFile = false;
  for (int k = 1; k < argc; ++k) {
    if (isWaitingForInputFile) {
      isWaitingForInputFile = false;
      if (validFileRead(argv[k])) {
        free(*inputFileName);
        *inputFileName = malloc(strlen(argv[k]) + 1);
        strcpy(*inputFileName, argv[k]);
      } else {
        fprintf(stderr, "le fichier n'est pas valide\n");
        return FUN_FAILURE;
      }
      continue;
    }
    if (isWaitingForGlosaryFile) {
      isWaitingForGlosaryFile = false;
      if (validFileRead(argv[k])) {
        free(*outputFileName);
        char *fileName = malloc(strlen(argv[k]) + 1);
        strcpy(fileName, argv[k]);
        list_put(glosaryFileNamesList, fileName);
      } else {
        fprintf(stderr, "le fichier n'est pas valide\n");
        return FUN_FAILURE;
      }
      continue;
    }
    if (isWaitingForOutputFile) {
      isWaitingForOutputFile = false;
      if (validFileWrite(argv[k])) {
        free(*outputFileName);
        *outputFileName = malloc(strlen(argv[k]) + 1);
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
      *upperCase = false;
      *lowerCase = false;
      continue;
    }
    if (strcmp(ARG_LONG_LOWER, argv[k]) == 0
        || strcmp(ARG_SHORT_LOWER, argv[k]) == 0) {
      *lowerCase = true;
      *upperCase = false;
      continue;
    }
    if (strcmp(ARG_LONG_UPPER, argv[k]) == 0
        || strcmp(ARG_SHORT_UPPER, argv[k]) == 0) {
      *upperCase = true;
      *lowerCase = false;
      continue;
    }
    if (checkArgFile(argv[k], ARG_LONG_INPUT, &offsetFile)) {
      if (validFileRead(argv[k] + offsetFile)) {
        free(*inputFileName);
        *inputFileName = malloc(strlen(argv[k] + offsetFile) + 1);
        strcpy(*inputFileName, argv[k] + offsetFile);
      } else {
        fprintf(stderr, "le fichier n'est pas valide\n");
        return FUN_FAILURE;
      }
      continue;
    }
    if (checkArgFile(argv[k], ARG_LONG_OUTPUT, &offsetFile)) {
      if (validFileWrite(argv[k] + offsetFile)) {
        free(*outputFileName);
        *outputFileName = malloc(strlen(argv[k] + offsetFile) + 1);
        strcpy(*outputFileName, argv[k] + offsetFile);
      } else {
        fprintf(stderr, "le fichier n'est pas valide\n");
        return FUN_FAILURE;
      }
      continue;
    }
    if (strcmp(ARG_SHORT_INPUT, argv[k]) == 0) {
      isWaitingForInputFile = true;
      continue;
    }
    if (strcmp(ARG_SHORT_OUPUT, argv[k]) == 0) {
      isWaitingForOutputFile = true;
      continue;
    }
    if (strcmp(ARG_FILE_GLOSARY, argv[k]) == 0) {
      isWaitingForGlosaryFile = true;
      continue;
    }
    //Si aucun test n'est passé, on essaye de nous faire passer un nouveau mot
    //pour le lexique anonyme
    addCorrectsWords(anonymGlosaryWords, argv[k]);
  }
  return FUN_SUCCESS;
}

bool checkArgFile(const char *arg, const char *value, size_t *k) {
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

void addCorrectsWords(list *anonymGlosaryWords, char *arg) {
  size_t k = 0;
  size_t index = 0;
  char w[WORD_LENGTH_MAX + 1];
  w[0] = '\0';
  while (arg[k] != '\0') {
    if (ispunct(arg[k]) || isspace(arg[k])) {
      if (index != 0) {
        char *t = malloc(index + 1);
        w[index] = '\0';
        strcpy(t, w);
        list_put(anonymGlosaryWords, t);
        index = 0;
      }
    } else {
      if (index < WORD_LENGTH_MAX) {
        w[index] = arg[k];
        ++index;
      }
    }
    ++k;
  }
  if (index != 0) {
    char *t = malloc(index + 1);
    w[index] = '\0';
    strcpy(t, w);
    list_put(anonymGlosaryWords, t);
  }
}

bool validFileRead(char *filename) {
  FILE *f = fopen(filename, "r");
  if (f == NULL) {
    return false;
  }
  fclose(f);
  return true;
}

bool validFileWrite(char *filename) {
  FILE *f = fopen(filename, "w");
  if (f == NULL) {
    return false;
  }
  fclose(f);
  return true;
}

void help() {
  printf("List of commands: \n");
  printf("--h             Display this help\n");
  printf("--sort ou -S    Affiche la liste dans l’ordre lexicographique.\n");
  printf("--input=FILE    Au lieu de l’entrée standard, lit le texte");
  printf(" dans le ﬁchier FILE;\n");
  printf("--ouput=FILE    Au lieu de la sortie standard, écrit le résultat");
  printf(" dans le ﬁchier FILE;\n");
  printf("--case=VALUE    Convertit tous les caractères lus selon VALUE :\n");
  printf("\t VALUE = lower : en minuscule\n");
  printf("\t VALUE = upper : en majuscule\n");
  printf("\t VALUE = as-is : laisse inchangé tous les caractères\n");
  printf("-l : équivalent à --case=lower;\n");
  printf("-u : équivalent à --case=upper\n");
  printf("-s : équivalent à --case=as-is\n");
  printf("-i : équivalent à --input=FILE\n");
  printf("-o : équivalent à --output=FILE\n");
  exit(EXIT_SUCCESS);
}
