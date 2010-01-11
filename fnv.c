#include "fnv.h"

/**
 * private function
 */
static uint_t fnv_hash(const char *k);
static fnv_t *fnv_ent_create();
static fnv_t *fnv_get_tail(fnv_t *ent, const char *k, uint_t klen);
static void fnv_ent_init(fnv_t *ent, const char *k, const char *v);

void fnv_tbl_init(fnv_t *tbl, uint_t c) {
  for (int i=0;i<c;++i) {
    fnv_ent_init(&tbl[i], NULL, NULL);
  }
}

char *fnv_get(fnv_t *tbl, const char *k, uint_t klen) {
  uint_t h = fnv_hash(k);
  if (tbl[h].key == NULL) {
    return NULL;
  }
  if (strncmp(tbl[h].key, k, klen) == 0) {
    return tbl[h].val;
  } else {
    fnv_t *tail = &tbl[h];
    while (tail->next) {
      tail = tail->next;
      if (strncmp(tail->key, k, klen) == 0) {
        return tail->val;
      }
    }
  }
  return NULL;
}

int fnv_put(fnv_t *tbl, const char *k, const char *v, uint_t klen) {
  uint_t h = fnv_hash(k);
  if (tbl[h].key != NULL) {
    if (strncmp(tbl[h].key, k, klen) == 0) {
      return FNV_PUT_DUPLICATE;
    }
    fnv_t *tail = fnv_get_tail(&tbl[h], k, klen);
    if (!tail) {
      return FNV_PUT_DUPLICATE;
    }
    tail->next = fnv_ent_create();
    fnv_ent_init(tail->next, k, v);
  } else {
    fnv_ent_init(&tbl[h], k, v);
  }
  return FNV_PUT_SUCCESS;
}

int fnv_out(fnv_t *tbl, const char *k, uint_t klen) {
  uint_t h    = fnv_hash(k);
  fnv_t *tail = &tbl[h];
  if (!tail || tail->key == NULL) {
    return FNV_OUT_NOTFOUND;
  }
  if (strncmp(tail->key, k, klen) == 0) {
    tail->key = NULL;
    tail->val = NULL;
    if (tail->next) {
      *tail = *(tail->next);
    }
  } else {
    while (tail->next) {
      fnv_t *current = tail;
      tail = tail->next;
      if (strncmp(tail->key, k, klen) == 0) {
        tail->key = NULL;
        tail->val = NULL;
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

void fnv_tbl_destroy(fnv_t *tbl, uint_t c) {
  for (int i=0;i<c;++i) {
    fnv_t *tail = tbl[i].next;
    while (tail) {
      fnv_t *current = tail;
      tail           = tail->next;
      FNV_FREE(current);
    }
  }
}

void fnv_tbl_print(fnv_t *tbl, uint_t c) {
  for (int i=0;i<c;++i) {
    fnv_t ent = tbl[i];
    if (ent.key == NULL) continue;
    printf("i = %d, key = %s, val = %s", i, tbl[i].key, tbl[i].val);
    if (ent.next != NULL) {
      fnv_t *tail = ent.next;
      while (tail) {
        printf(" -> key = %s, val = %s", tail->key, tail->val);
        tail =tail->next;
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
static uint_t fnv_hash(const char *k) {
  uint_t h = FNV_OFFSET_BASIS;
  for (uchar_t *p=(uchar_t *)k;*p!='\0';++p) {
    h *= FNV_PRIME;
    h ^= *p;
  }
  return h % FNV_TBL_CNT;
}

static void fnv_ent_init(fnv_t *ent, const char *k, const char *v) {
  ent->key  = (char *)k;
  ent->val  = (char *)v;
  ent->next = NULL;
}

static fnv_t *fnv_get_tail(fnv_t *ent, const char *k, uint_t klen) {
  fnv_t *tail = ent;
  while (tail->next) {
    tail = tail->next;
    if (strncmp(tail->key, k, klen) == 0) {
      return NULL;
    }
  }
  return tail;
}

static fnv_t *fnv_ent_create() {
  fnv_t *ent;
  FNV_MALLOC(ent, sizeof(fnv_t));
  return ent;
}
