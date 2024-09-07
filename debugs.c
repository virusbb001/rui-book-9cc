#include <stdio.h>
#include "9cc.h"

const char* TokenKindName[TK_EOF+1] = {
  "TK_RESERVED",
  "TK_IDENT",
  "TK_NUM",
  "TK_RETURN",   // return
  "TK_IF",       // if
  "TK_ELSE",     // else
  "TK_WHILE",    // while
  "TK_FOR",      // for
  "TK_EOF",
};

const char* NodeKindName[ND_RETURN+1] = {
  "ND_NOT_SET", // 誤の防止
  "ND_ADD", // +
  "ND_SUB", // -
  "ND_MUL", // *
  "ND_DIV", // /
  "ND_NUM", // 整数
  "ND_EQ",  // ==
  "ND_NE",  // !=
  "ND_LT",  // <
  "ND_LEQ", // <=
  "ND_GT",  // >
  "ND_GEQ", // >=
  "ND_ASSIGN", // =
  "ND_LVAR", // ローカル変数
  "ND_IF",
  "ND_WHILE",
  "ND_FOR",
  "ND_BLOCK",
  "ND_CALL", // 関数呼び出し
  "ND_RETURN", // return
};

void print_tokens(Token *tok) {
  Token *cur = tok;

  while(cur) {
    fprintf(stderr, "%s (%p)", TokenKindName[cur->kind], cur->str);
    if (cur->kind == TK_IDENT) {
      fprintf(stderr, " %.*s", cur->len, cur->str);
    }
    fputc('\n', stderr);
    cur = cur->next;
  }
}
