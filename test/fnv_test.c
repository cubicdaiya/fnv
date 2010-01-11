#include <stdio.h>
#include <CUnit/CUnit.h>
#include <CUnit/Console.h>
#include "../fnv.h"

/**
 *  Z80 instructions (test data)
 */
const char *test_keys[] = {
  "LD",
  "PUSH",
  "POP",
  "EX",
  "EXX",
  "LDI",
  "LDIR",
  "LDD",
  "LDDR",
  "CPI",
  "CPIR",
  "CPD",
  "CPDR",
  "ADD",
  "ADC",
  "SUB",
  "SBC",
  "CP",
  "INC",
  "DEC",
  "AND",
  "OR",
  "XOR",
  "DAA",
  "CPL",
  "NEG",
  "CCF",
  "SCF",
  "NOP",
  "HALT",
  "DI",
  "EI",
  "IM",
  "RLCA",
  "RLA",
  "RRCA",
  "RRA",
  "RLC",
  "RL",
  "RRC",
  "RR",
  "SLR",
  "SRL",
  "SRA",
  "RLD",
  "RRD",
  "BIT",
  "SET",
  "RES",
  "JP",
  "JR",
  "DJMZ",
  "CALL",
  "RET",
  "RETN",
  "RST",
  "IN",
  "INI",
  "INIR",
  "IND",
  "INDR",
  "OUT",
  "OUTI",
  "OTIR",
  "OUTD",
  "OTDR",
  "ORG",
  "EQU",
  "END",
  "DEFB",
  "DEFW",
  "DEFS",
};

static void test_fnv_tbl_init(fnv_t *tbl, uint_t c);

static void test_fnv_get(void);
static void test_fnv_get_notfound(void);
static void test_fnv_put_success(void);
static void test_fnv_put_duplicate(void);
static void test_fnv_out_success(void);
static void test_fnv_out_notfound(void);

static void test_fnv_tbl_init(fnv_t *tbl, uint_t c) {
  for (int i=0;i<c;++i) {
    fnv_put(tbl, test_keys[i], test_keys[i], strlen(test_keys[i]));
  }
}

static void test_fnv_get(void) {
  fnv_t tbl[FNV_TBL_CNT];
  uint_t c = sizeof(test_keys) / sizeof(char *);
  fnv_tbl_init(tbl, FNV_TBL_CNT);
  test_fnv_tbl_init(tbl, c);
  for (int i=0;i<c;++i) {
    CU_ASSERT(fnv_get(tbl, test_keys[i], strlen(test_keys[i])) == (char *)test_keys[i]);
  }
  fnv_tbl_destroy(tbl, FNV_TBL_CNT);
}

static void test_fnv_get_notfound(void) {
  fnv_t tbl[FNV_TBL_CNT];
  uint_t c = sizeof(test_keys) / sizeof(char *);
  fnv_tbl_init(tbl, FNV_TBL_CNT);
  test_fnv_tbl_init(tbl, c);
  CU_ASSERT(fnv_get(tbl, "PUSHD", strlen("PUSHD")) == NULL);
  fnv_tbl_destroy(tbl, FNV_TBL_CNT);
}

static void test_fnv_put_success(void) {
  fnv_t tbl[FNV_TBL_CNT];
  uint_t c = sizeof(test_keys) / sizeof(char *);
  fnv_tbl_init(tbl, FNV_TBL_CNT);
  test_fnv_tbl_init(tbl, c);
  CU_ASSERT(fnv_put(tbl, "PUSHD", "POPD", strlen("PUSHD")) == FNV_PUT_SUCCESS);
  fnv_tbl_destroy(tbl, FNV_TBL_CNT);
}

static void test_fnv_put_duplicate(void) {
  fnv_t tbl[FNV_TBL_CNT];
  uint_t c = sizeof(test_keys) / sizeof(char *);
  fnv_tbl_init(tbl, FNV_TBL_CNT);
  test_fnv_tbl_init(tbl, c);
  CU_ASSERT(fnv_put(tbl, "LD", "LDD", strlen("LD")) == FNV_PUT_DUPLICATE);
  fnv_tbl_destroy(tbl, FNV_TBL_CNT);
}

static void test_fnv_out_success(void) {
  fnv_t tbl[FNV_TBL_CNT];
  uint_t c = sizeof(test_keys) / sizeof(char *);
  fnv_tbl_init(tbl, FNV_TBL_CNT);
  test_fnv_tbl_init(tbl, c);
  CU_ASSERT(fnv_out(tbl, "LD",   strlen("LD"))   == FNV_OUT_SUCCESS);
  CU_ASSERT(fnv_out(tbl, "OUTI", strlen("OUTI")) == FNV_OUT_SUCCESS);
  CU_ASSERT(fnv_out(tbl, "EI",   strlen("EI"))   == FNV_OUT_SUCCESS);
  fnv_tbl_destroy(tbl, FNV_TBL_CNT);
}

static void test_fnv_out_notfound(void) {
  fnv_t tbl[FNV_TBL_CNT];
  uint_t c = sizeof(test_keys) / sizeof(char *);
  fnv_tbl_init(tbl, FNV_TBL_CNT);
  test_fnv_tbl_init(tbl, c);
  CU_ASSERT(fnv_out(tbl, "PUSHD", strlen("PUSHD")) == FNV_OUT_NOTFOUND);
  fnv_tbl_destroy(tbl, FNV_TBL_CNT);
}

int main (int argc, char *argv[]) {
  
  CU_pSuite suite_fnv;
  CU_initialize_registry();
  suite_fnv = CU_add_suite("fnv", NULL, NULL);
  CU_add_test(suite_fnv, "test_fnv_get", test_fnv_get);
  CU_add_test(suite_fnv, "test_fnv_get_notfound", test_fnv_get_notfound);
  CU_add_test(suite_fnv, "test_fnv_put_success", test_fnv_put_success);
  CU_add_test(suite_fnv, "test_fnv_put_duplicate", test_fnv_put_duplicate);
  CU_add_test(suite_fnv, "test_fnv_out_success", test_fnv_out_success);
  CU_add_test(suite_fnv, "test_fnv_out_notfound", test_fnv_out_notfound);

  CU_console_run_tests();
  CU_cleanup_registry();
  
  return 0;
}
