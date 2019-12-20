#include <stdlib.h>
#include <ctype.h>
#include "digs.h"

#define FUN_SUCCESS 0
#define FUN_FAILURE 1

int string_fput(const digs *ptr, FILE *stream) {

}

int string_compar(const char* *ptr1, const char* *ptr2) {
  return strcmp(*ptr1, *ptr2);
}
