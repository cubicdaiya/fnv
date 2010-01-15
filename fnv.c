#include "fnv.h"

/**
 * private function
 */
static uint_t fnv_hash(fnv_tbl_t *tbl, const char *k);
static fnv_ent_t *fnv_ent_create();
static fnv_ent_t *fnv_get_tail(fnv_ent_t *ent, const char *k, size_t ksiz);
static void fnv_ent_init(fnv_ent_t *ent, const char *k, const char *v);

fnv_tbl_t *fnv_tbl_create(fnv_ent_t *ents, size_t c) {
  fnv_tbl_t *tbl;
  FNV_MALLOC(tbl, sizeof(fnv_tbl_t));
  tbl->ents = ents;
  tbl->c    = c;
  for (int i=0;i<c;++i) {
    fnv_ent_init(&ents[i], NULL, NULL);
  }
  return tbl;
}

char *fnv_get(fnv_tbl_t *tbl, const char *k, size_t ksiz) {
  FNV_CHKOVERSIZ(ksiz, FNV_KEY_MAX_LENGTH, NULL);
  fnv_ent_t *ents = tbl->ents;
  uint_t h = fnv_hash(tbl, k);
  if (ents[h].k == NULL) {
    return NULL;
  }
  if (strcmp(ents[h].k, k) == 0) {
    return ents[h].v;
  } else {
    fnv_ent_t *tail = &ents[h];
    while (tail->next) {
      tail = tail->next;
      if (strcmp(tail->k, k) == 0) {
        return tail->v;
      }
    }
  }
  return NULL;
}

int fnv_put(fnv_tbl_t *tbl, const char *k, const char *v, size_t ksiz, size_t vsiz) {
  FNV_CHKOVERSIZ(ksiz, FNV_KEY_MAX_LENGTH, FNV_PUT_OVER_KEYSIZ);
  FNV_CHKOVERSIZ(vsiz, FNV_VAL_MAX_LENGTH, FNV_PUT_OVER_VALSIZ);
  uint_t h = fnv_hash(tbl, k);
  fnv_ent_t *ents = tbl->ents;
  if (ents[h].k != NULL) {
    if (strcmp(ents[h].k, k) == 0) {
      return FNV_PUT_DUPLICATE;
    }
    fnv_ent_t *tail = fnv_get_tail(&ents[h], k, ksiz);
    if (!tail) {
      return FNV_PUT_DUPLICATE;
    }
    tail->next = fnv_ent_create();
    fnv_ent_init(tail->next, k, v);
  } else {
    fnv_ent_init(&ents[h], k, v);
  }
  return FNV_PUT_SUCCESS;
}

int fnv_out(fnv_tbl_t *tbl, const char *k, size_t ksiz) {
  FNV_CHKOVERSIZ(ksiz, FNV_KEY_MAX_LENGTH, FNV_OUT_OVER_KEYSIZ);
  uint_t h = fnv_hash(tbl, k);
  fnv_ent_t *tail = &tbl->ents[h];
  if (!tail || tail->k == NULL) {
    return FNV_OUT_NOTFOUND;
  }
  if (strcmp(tail->k, k) == 0) {
    tail->k = NULL;
    tail->v = NULL;
    if (tail->next) {
      *tail = *(tail->next);
    }
  } else {
    while (tail->next) {
      fnv_ent_t *current = tail;
      tail = tail->next;
      if (strcmp(tail->k, k) == 0) {
        tail->k = NULL;
        tail->v = NULL;
        if (tail->next) {
          current->next = tail->next;
        }
        return FNV_OUT_SUCCESS;
      }
    }
    return FNV_OUT_NOTFOUND;
  }
  return FNV_OUT_SUCCESS;
}

void fnv_tbl_destroy(fnv_tbl_t *tbl) {
  fnv_ent_t *ents = tbl->ents;
  size_t c = tbl->c;
  for (int i=0;i<c;++i) {
    fnv_ent_t *tail = ents[i].next;
    while (tail) {
      fnv_ent_t *current = tail;
      tail = tail->next;
      FNV_FREE(current);
    }
  }
  FNV_FREE(tbl);
}

void fnv_tbl_print(fnv_tbl_t *tbl, size_t c) {
  fnv_ent_t *ents = tbl->ents;
  for (int i=0;i<c;++i) {
    fnv_ent_t *ent = &ents[i];
    if (ent->k == NULL) continue;
    printf("i = %d, key = %s, val = %s", i, ent->k, ent->v);
    if (ent->next != NULL) {
      fnv_ent_t *tail = ent->next;
      while (tail) {
        printf(" -> key = %s, val = %s", tail->k, tail->v);
        tail = tail->next;
      }
    }
    printf("\n");
  }
}

/* following is private function */ 

/**
 * The algorithm implemented here is based on FNV hash algorithm
 * described by Glenn Fowler, Phong Vo, Landon Curt Noll
 */
static uint_t fnv_hash(fnv_tbl_t *tbl, const char *k) {
  uint_t h = FNV_OFFSET_BASIS;
  for (uchar_t *p=(uchar_t *)k;*p!='\0';++p) {
    h *= FNV_PRIME;
    h ^= *p;
  }
  return h % tbl->c;
}

static void fnv_ent_init(fnv_ent_t *ent, const char *k, const char *v) {
  ent->k  = (char *)k;
  ent->v  = (char *)v;
  ent->next = NULL;
}

static fnv_ent_t *fnv_get_tail(fnv_ent_t *ent, const char *k, size_t ksiz) {
  fnv_ent_t *tail = ent;
  while (tail->next) {
    tail = tail->next;
    if (strcmp(tail->k, k) == 0) {
      return NULL;
    }
  }
  return tail;
}

static fnv_ent_t *fnv_ent_create() {
  fnv_ent_t *ent;
  FNV_MALLOC(ent, sizeof(fnv_ent_t));
  return ent;
}

