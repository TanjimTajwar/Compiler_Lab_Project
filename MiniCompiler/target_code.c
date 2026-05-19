/*
 * File: target_code.c
 * Developed by: Monir Hossain (ID: 21701009)
 * Contribution: Register-Based Pseudo Assembly Generation
 *
 * Complexity: O(n) over TAC instructions; register map O(r) temps.
 */

#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct RegMap {
    char name[64];
    char reg[8];
    struct RegMap *next;
} RegMap;

static int reg_num = 0;
static RegMap *regmap = NULL;

static const char *alloc_reg(const char *sym) {
  static char buf[8];
  for (RegMap *p = regmap; p; p = p->next) {
    if (strcmp(p->name, sym) == 0) return p->reg;
  }
  RegMap *m = (RegMap *)calloc(1, sizeof(RegMap));
  strncpy(m->name, sym, sizeof(m->name) - 1);
  snprintf(m->reg, sizeof(m->reg), "r%d", reg_num++);
  m->next = regmap;
  regmap = m;
  return m->reg;
}

static void free_regmap(void) {
  while (regmap) {
    RegMap *n = regmap->next;
    free(regmap);
    regmap = n;
  }
  reg_num = 0;
}

static int is_imm(const char *s) {
  if (!s || !*s) return 0;
  if (*s == '-') s++;
  while (*s) {
    if (!isdigit((unsigned char)*s)) return 0;
    s++;
  }
  return 1;
}

static void emit_load(FILE *fp, const char *reg, const char *val) {
  if (is_imm(val))
    fprintf(fp, "    LI %s, %s\n", reg, val);
  else
    fprintf(fp, "    LOAD %s, %s\n", reg, val);
}

int target_generate(TACList *tac, const char *path) {
  FILE *fp = fopen(path, "w");
  if (!fp) {
    perror("output.asm");
    return -1;
  }

  free_regmap();
  fprintf(fp, "; Pseudo Assembly (MiniLang)\n");
  fprintf(fp, ".text\n");
  fprintf(fp, "main:\n");

  for (TACInstr *p = tac->head; p; p = p->next) {
    if (p->dead) continue;

    if (p->is_label) {
      fprintf(fp, "%s:\n", p->result);
      continue;
    }

    if (!strcmp(p->op, "halt")) {
      fprintf(fp, "    HALT\n");
      continue;
    }
    if (!strcmp(p->op, "LABEL")) continue;

    if (!strcmp(p->op, "=")) {
      const char *rd = alloc_reg(p->result);
      if (is_imm(p->arg1))
        fprintf(fp, "    LI %s, %s\n", rd, p->arg1);
      else {
        const char *rs = alloc_reg(p->arg1);
        fprintf(fp, "    MOV %s, %s\n", rd, rs);
      }
      continue;
    }

    if (!strcmp(p->op, "param")) {
      const char *rs = alloc_reg(p->arg1);
      fprintf(fp, "    PARAM %s\n", rs);
      continue;
    }
    if (!strcmp(p->op, "call")) {
      fprintf(fp, "    CALL print\n");
      continue;
    }
    if (!strcmp(p->op, "goto")) {
      fprintf(fp, "    JMP %s\n", p->result);
      continue;
    }
    if (!strcmp(p->op, "if_false")) {
      const char *rs = alloc_reg(p->arg1);
      fprintf(fp, "    BEQZ %s, %s\n", rs, p->result);
      continue;
    }

    /* binary / unary arithmetic */
    const char *rd = alloc_reg(p->result);
    const char *r1 = alloc_reg(p->arg1);
    const char *r2 = p->arg2 ? alloc_reg(p->arg2) : NULL;

    if (is_imm(p->arg1)) {
      fprintf(fp, "    LI %s, %s\n", r1, p->arg1);
    }
    if (p->arg2 && is_imm(p->arg2)) {
      char tmp[8];
      snprintf(tmp, sizeof(tmp), "r%d", reg_num++);
      fprintf(fp, "    LI %s, %s\n", tmp, p->arg2);
      r2 = tmp;
    }

    if (!strcmp(p->op, "+"))
      fprintf(fp, "    ADD %s, %s, %s\n", rd, r1, r2);
    else if (!strcmp(p->op, "-"))
      fprintf(fp, "    SUB %s, %s, %s\n", rd, r1, r2);
    else if (!strcmp(p->op, "*"))
      fprintf(fp, "    MUL %s, %s, %s\n", rd, r1, r2);
    else if (!strcmp(p->op, "/"))
      fprintf(fp, "    DIV %s, %s, %s\n", rd, r1, r2);
    else if (!strcmp(p->op, "<"))
      fprintf(fp, "    SLT %s, %s, %s\n", rd, r1, r2);
    else if (!strcmp(p->op, ">"))
      fprintf(fp, "    SGT %s, %s, %s\n", rd, r1, r2);
    else if (!strcmp(p->op, "=="))
      fprintf(fp, "    SEQ %s, %s, %s\n", rd, r1, r2);
    else if (!strcmp(p->op, "!="))
      fprintf(fp, "    SNE %s, %s, %s\n", rd, r1, r2);
    else if (!strcmp(p->op, "&&"))
      fprintf(fp, "    AND %s, %s, %s\n", rd, r1, r2);
    else if (!strcmp(p->op, "||"))
      fprintf(fp, "    OR %s, %s, %s\n", rd, r1, r2);
    else if (!strcmp(p->op, "neg"))
      fprintf(fp, "    NEG %s, %s\n", rd, r1);
    else if (!strcmp(p->op, "!"))
      fprintf(fp, "    NOT %s, %s\n", rd, r1);
  }

  fclose(fp);
  free_regmap();
  return 0;
}
