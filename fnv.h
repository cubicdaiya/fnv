
#ifndef FNV_H
#define FNV_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FNV_MALLOC(p, n)               \
  do {                                 \
    if (((p) = malloc(n)) == NULL) {   \
      printf("malloc failed");         \
      exit(-1);                        \
    }                                  \
  } while(false)

#define FNV_FREE(p)                             \
  do {                                          \
    free(p);                                    \
    (p) = NULL;                                 \
  } while(false)

#define FNV_CHKOVERSIZ(siz, max, ret)              \
  do {                                            \
    if (siz > max) {                              \
      return ret;                                 \
    }                                             \
  } while(false)
/**
 * config
 */
#define FNV_TBL_CNT_DEFAULT 1023
#define FNV_PRIME 0x01000193
#define FNV_OFFSET_BASIS 0x811c9dc5

#define FNV_KEY_MAX_LENGTH 1024
#define FNV_VAL_MAX_LENGTH 1024

typedef int fnv_type_t;
#define FNV_TYPE_KEY 1
#define FNV_TYPE_VAL 2

#define FNV_PUT_SUCCESS   1
#define FNV_PUT_DUPLICATE 2

#define FNV_PUT_OVER_KEYSIZ -1
#define FNV_PUT_OVER_VALSIZ -2

#define FNV_OUT_SUCCESS   1
#define FNV_OUT_NOTFOUND  -1
#define FNV_OUT_OVER_KEYSIZ -2

typedef unsigned int uint_t;
typedef unsigned char uchar_t;

typedef struct fnv_ent_s {
  char *k;
  char *v;
  struct fnv_ent_s *next;
} fnv_ent_t;

typedef struct fnv_tbl_s {
  fnv_ent_t *ents;
  size_t c;
} fnv_tbl_t;

fnv_tbl_t *fnv_tbl_create(fnv_ent_t *ents, size_t c);
void fnv_tbl_init(fnv_tbl_t *tbl, size_t c);
char *fnv_get(fnv_tbl_t *tbl, const char *k, size_t ksiz);
int fnv_put(fnv_tbl_t *tbl, const char *k, const char *v, size_t ksiz, size_t vsiz);
int fnv_out(fnv_tbl_t *tbl, const char *k, size_t ksiz);
void fnv_tbl_destroy(fnv_tbl_t *tbl);
void fnv_tbl_print(fnv_tbl_t *tbl, size_t c);

#endif // FNV_H
