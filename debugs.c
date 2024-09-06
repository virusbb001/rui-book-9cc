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
