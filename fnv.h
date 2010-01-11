
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


/**
 * config
 */
#define FNV_TBL_CNT 1024
#define FNV_PRIME 0x01000193
#define FNV_OFFSET_BASIS 0x811c9dc5

#define FNV_PUT_SUCCESS   1
#define FNV_PUT_DUPLICATE 2

#define FNV_OUT_SUCCESS   1
#define FNV_OUT_NOTFOUND  -1


typedef unsigned int uint_t;
typedef unsigned char uchar_t;
typedef struct fnv_s {
  char *key;
  char *val;
  struct fnv_s *next;
} fnv_t;

void fnv_tbl_init(fnv_t *tbl, uint_t c);
char *fnv_get(fnv_t *tbl, const char *k, uint_t klen);
int fnv_put(fnv_t *tbl, const char *k, const char *v, uint_t klen, uint_t vlen);
int fnv_out(fnv_t *tbl, const char *k, uint_t klen);
void fnv_tbl_destroy(fnv_t *tbl, uint_t c);
void fnv_tbl_print(fnv_t *tbl, uint_t c);

#endif // FNV_H
