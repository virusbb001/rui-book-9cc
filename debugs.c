#include <stdio.h>
#include "9cc.h"

const char* TokenKindName[TK_EOF+1] = {
  "TK_RESERVED",
  "TK_IDENT",
  "TK_NUM",
  "TK_EOF",
};

void print_tokens(Token *tok) {
  Token *cur = tok;

  while(cur) {
    fprintf(stderr, "%s (%p)\n", TokenKindName[cur->kind], cur->str);
    cur = cur->next;
  }
}
