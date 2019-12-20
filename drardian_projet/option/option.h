#ifndef OPTIONS__H
#define OPTIONS__H

#include "option.c"

#define WORD_LENGTH_MAX 63
#define OPTION_FUN_FAILURE -1
#define OPTION_FUN_SUCCESS 0

int option(int argc, char** argv, bool* sort, bool* lowerCase, bool* upperCase,
    bool* asIsCase, list inputFileNamesList, list outputFileNamesList);
#endif
