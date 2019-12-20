#include "ldigs.h"
#include <stdlib.h>
#define FUN_FAILURE 1
#define FUN_SUCCESS 0

typedef struct cldigs cldigs;

struct cldigs {
  digs value;
  cldigs *next;
};

struct ldigs {
  cldigs *head;
};

struct digs {
  char* value;
}

digs digs_create(char * v) {
  digs* cell = malloc(sizeof(digs));
  cell->value = v;
}

void digs_dispose(digs** xptr) {
  free(*xptr);
  xptr = NULL;
}

ldigs *ldigs_empty(void) {
  ldigs *p = malloc(sizeof(*p));
  if (p == NULL) {
    return NULL;
  }
  p->head = NULL;
  return p;
}

bool ldigs_is_empty(const ldigs *s) {
  return s->head == NULL; 
}

int ldigs_head_value(const ldigs *s, digs *ptr) {
  if (ldigs_is_empty(s)) {
    return FUN_FAILURE;
  }
  *ptr = (s->head)->value;
  return FUN_SUCCESS;
}

int ldigs_insert_head(ldigs *s, digs *ptr) {
  cldigs *p = malloc(sizeof(*p));
  if (p == NULL) {
    return FUN_FAILURE;
  }
  p->value = *ptr;
  p->next = (s->head);
  s->head = p;
  return FUN_SUCCESS;
}

int ldigs_move_head_head(ldigs *src, ldigs *dest) {
  if (ldigs_is_empty(src)) {
    return FUN_FAILURE;
  }
  cldigs *q = dest->head;
  dest->head = src->head;
  src->head = src->head->next;
  dest->head->next = q;
  return FUN_SUCCESS;
}

int ldigs_fput(const ldigs *s, FILE *stream) {
  cldigs *p = s->head;
  if (p == NULL) {
    return FUN_FAILURE;
  }
  while (p != NULL) {
    digs_fput(&p->value, stream);
    fputc(' ', stream);
    p = p->next;
  }
  fputc('\n', stream);
  return FUN_SUCCESS;
}

void ldigs_dispose(ldigs **sptr) {
  if ((*sptr)->head == NULL) {
    free(*sptr);
    *sptr = NULL;
    return;
  }
  *->value
  cldigs *n = (*sptr)->head;
  (*sptr)->head = ((*sptr)->head)->next;
  free(n);
  ldigs_dispose(sptr);
}
