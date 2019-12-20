//  Implantation polymorphe pour la spécification TABLE du TDA Table(T, T') dans
//    le cas d'une table de hachage par chainage séparé

#include <stdlib.h>
#include <stdint.h>
#include "hashtable.h"

//  Les 24 premiers termes de la suite récurrente d'ordre 1 <c_{n}> définie par
//      c_{0} = 43 ;
//      c_{n} = 2 * c_{n - 1} + 87, pour n >= 1 ;
//    sont premiers (14 termes, soit 58 %) ou ne possèdent qu'un seul diviseur
//    non trivial (10 termes, soit 42 %). Voici la liste de ces termes,
//    l'éventuel diviseur non trivial (et donc nécessairement premier) étant
//    noté entre crochets :
//      43 173 433 953 1993 4073 8233 16553 33193[19] 66473[11] 133033 266153
//      532393[37] 1064873 2129833[43] 4259753 8519593[2767] 17039273[79]
//      34078633 68157353[11] 136314793[4801] 272629673 545259433[1181]
//      1090518953[307]

//  Le nombre m de compartiments du tableau de hachage s'aligne sur <c_{n}> :
//    initialement, m vaut 43 ; dès le taux de remplissage de la table de
//    hachage atteint un certain seuil, le tableau de hachage est agrandi à
//    43 * m + 87 compartiments

#define HT__NSLOTS_MIN  43
#define HT__ENLARGE_MUL 2
#define HT__ENLARGE_ADD 87

//  Le seuil est très proche mais toujours inférieur ou égal au quotient de la
//    division réelle de l'entier strictement positif HT__LDFACT_MAX_NUMER par
//    l'entier strictement positif HT__LDFACT_MAX_DENOM. Très exactement, le
//    tableau de hachage est agrandi pour toute demande d'ajout effectif pour
//    laquelle le nombre n d'entrées de la table de hachage est égal à
//    m / HT__LDFACT_MAX_DENOM * HT__LDFACT_MAX_NUMER, la division étant cette
//    fois entière

#define HT__LDFACT_MAX_NUMER 3
#define HT__LDFACT_MAX_DENOM 4

//  Les deux définitions précédentes représentent un seuil maximum de 0,75.
//    Elles peuvent être modifiées, par exemple en les fixant toutes les deux à
//    1 pour un seuil maximum de 1. La directive qui suit s'assure de la
//    cohérence du choix des valeurs. Elle ne doit pas être modifiée

#if (HT__LDFACT_MAX_NUMER < 1                                                  \
  || HT__LDFACT_MAX_DENOM < 1                                                  \
  || HT__NSLOTS_MIN * HT__ENLARGE_MUL + HT__ENLARGE_ADD < HT__LDFACT_MAX_DENOM)
#error Bad choice of HT__LDFACT_MAX_NUMER or HT__LDFACT_MAX_DENOM
#endif

//  struct hashtable, hashtable : gestion du chainage séparé par liste dynamique
//    simplement chainée. Le composant compar mémorise la fonction de
//    comparaison des clés, hashfun, leur fonction de hachage. Le tableau de
//    hachage est alloué dynamiquement ; son adresse et sa longueur sont
//    mémorisées par les composants hasharray et nslots. Les composants
//    nentriesmax et nentries mémorisent respectivement le maximum du nombre
//    d'entrées supportables dans la configuration courante le nombre d'entrées

//  Si une demande d'ajout d'une nouvelle entrée ne donne pas lieu un
//    agrandissement de la table de hachage, l'ajout a lieu en queue de liste.
//    Dans le cas contraire, les cellules sont redistribuées vers la nouvelle
//    table avec insertion en tête de liste, y compris ensuite pour la nouvelle
//    cellule

typedef struct cell cell;

struct cell {
  const void *keyptr;
  const void *valueptr;
  cell *next;
};

struct hashtable {
  int (*compar)(const void *, const void *);
  size_t (*hashfun)(const void *);
  cell **hasharray;
  size_t nslots;
  size_t nentriesmax;
  size_t nentries;
};

hashtable *hashtable_empty(int (*compar)(const void *, const void *),
    size_t (*hashfun)(const void *)) {
  hashtable *ht = malloc(sizeof(hashtable));
  if (ht == NULL) {
    return NULL;
  }
  ht->compar = compar;
  ht->hashfun = hashfun;
  ht->hasharray = malloc(HT__NSLOTS_MIN * sizeof(cell *));
  if (ht->hasharray == NULL) {
    free(ht);
    return NULL;
  }
  for (size_t k = 0; k < HT__NSLOTS_MIN; ++k) {
    ht->hasharray[k] = NULL;
  }
  ht->nslots = HT__NSLOTS_MIN;
  ht->nentriesmax =
      (size_t) (HT__NSLOTS_MIN / HT__LDFACT_MAX_DENOM * HT__LDFACT_MAX_NUMER);
  ht->nentries = 0;
  return ht;
}

static cell **hashtable__search_slot(const hashtable *ht, const void *keyptr) {
  return &ht->hasharray[ht->hashfun(keyptr) % ht->nslots];
}

static cell **hashtable__search_rank(const hashtable *ht, const void *keyptr) {
  cell **pp = hashtable__search_slot(ht, keyptr);
  while (*pp != NULL && ht->compar((*pp)->keyptr, keyptr) != 0) {
    pp = &(*pp)->next;
  }
  return pp;
}

static int hashtable__enlarge(hashtable *ht) {
  if (ht->nslots > (SIZE_MAX / sizeof(cell *) - HT__ENLARGE_ADD)
      / HT__ENLARGE_MUL) {
    return 1;
  }
  size_t m = ht->nslots * HT__ENLARGE_MUL + HT__ENLARGE_ADD;
  if (m > SIZE_MAX / HT__LDFACT_MAX_NUMER * HT__LDFACT_MAX_DENOM) {
    return 1;
  }
  size_t n = m / HT__LDFACT_MAX_DENOM * HT__LDFACT_MAX_NUMER;
  cell **a = malloc(m * sizeof(cell *));
  if (a == NULL) {
    return 1;
  }
  for (size_t k = 0; k < m; ++k) {
    a[k] = NULL;
  }
  for (size_t k = 0; k < ht->nslots; ++k) {
    cell *p = ht->hasharray[k];
    while (p != NULL) {
      cell **pp = &a[ht->hashfun(p->keyptr) % m];
      cell *t = p;
      p = t->next;
      t->next = *pp;
      *pp = t;
    }
  }
  free(ht->hasharray);
  ht->hasharray = a;
  ht->nslots = m;
  ht->nentriesmax = n;
  return 0;
}

const void *hashtable_add(hashtable *ht, const void *keyptr,
    const void *valueptr) {
  if (valueptr == NULL) {
    return NULL;
  }
  cell **pp = hashtable__search_rank(ht, keyptr);
  if (*pp != NULL) {
    (*pp)->valueptr = valueptr;
  } else {
    if (ht->nentries == ht->nentriesmax) {
      if (hashtable__enlarge(ht) != 0) {
        return NULL;
      }
      pp = hashtable__search_slot(ht, keyptr);
    }
    cell *p = malloc(sizeof(cell));
    if (p == NULL) {
      return NULL;
    }
    p->keyptr = keyptr;
    p->valueptr = valueptr;
    p->next = *pp;
    *pp = p;
    ht->nentries += 1;
  }
  return valueptr;
}

const void *hashtable_remove(hashtable *ht, const void *keyptr) {
  cell **pp = hashtable__search_rank(ht, keyptr);
  if (*pp == NULL) {
    return NULL;
  }
  cell *t = *pp;
  const void *valueptr = t->valueptr;
  *pp = t->next;
  free(t);
  ht->nentries -= 1;
  return valueptr;
}

const void *hashtable_value(const hashtable *ht, const void *keyptr) {
  cell *p = *hashtable__search_rank(ht, keyptr);
  return p == NULL ? NULL : p->valueptr;
}

void hashtable_dispose(hashtable **htptr) {
  if (*htptr == NULL) {
    return;
  }
  for (size_t k = 0; k < (*htptr)->nslots; ++k) {
    cell *p = (*htptr)->hasharray[k];
    while (p != NULL) {
      cell *t = p;
      p = p->next;
      free(t);
    }
  }
  free((*htptr)->hasharray);
  free(*htptr);
  *htptr = NULL;
}

#ifdef HASHTABLE_CHECKUP

void hashtable_get_checkup(const hashtable *ht,
    struct hashtable_checkup *htcuptr) {
  size_t m = 0;
  double s = 0.0;
  for (size_t k = 0; k < ht->nslots; ++k) {
    size_t n = 0;
    const cell *p = ht->hasharray[k];
    while (p != NULL) {
      ++n;
      p = p->next;
    }
    if (n > m) {
      m = n;
    }
    s += (double) n * (double) (n + 1) / 2.0;
  }
  double ldfactcurr = (double) ht->nentries / (double) ht->nslots;
  *htcuptr = (struct hashtable_checkup) {
    .nslots = ht->nslots,
    .nentries = ht->nentries,
    .ldfactmax = 1.0 / HT__LDFACT_MAX_DENOM * HT__LDFACT_MAX_NUMER,
    .ldfactcurr = ldfactcurr,
    .maxlen = m,
    .postheo = ht->nentries == 0
        ? 0.0
        : 1.0 + (ldfactcurr - 1.0 / (double) ht->nslots) / 2.0,
    .poscurr = s / (double) ht->nentries,
  };
}

#define P_TITLE(textstream, name) \
  fprintf(textstream, "--- Info: %s\n", name)
#define P_VALUE(textstream, name, format, value) \
  fprintf(textstream, "%12s\t" format "\n", name, value)

int hashtable_display_checkup(const hashtable *ht, FILE *textstream) {
  struct hashtable_checkup htcu;
  hashtable_get_checkup(ht, &htcu);
  return 0 > P_TITLE(textstream, "Hashtable checkup")
      || 0 > P_VALUE(textstream, "n.slots", "%zu", htcu.nslots)
      || 0 > P_VALUE(textstream, "n.entries", "%zu", htcu.nentries)
      || 0 > P_VALUE(textstream, "ld.fact.max", "%lf", htcu.ldfactmax)
      || 0 > P_VALUE(textstream, "ld.fact.curr", "%lf", htcu.ldfactcurr)
      || 0 > P_VALUE(textstream, "max.len", "%zu", htcu.maxlen)
      || 0 > P_VALUE(textstream, "pos.theo", "%lf", htcu.postheo)
      || 0 > P_VALUE(textstream, "pos.curr", "%lf", htcu.poscurr);
}

#endif
