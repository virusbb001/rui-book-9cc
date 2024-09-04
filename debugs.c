#include <stdio.h>
#include "9cc.h"

const char* TokenKindName[TK_EOF+1] = {
  "TK_RESERVED",
  "TK_NUM",
  "TK_EOF",
};

void print_tokens(Token *tok) {
  Token *cur = tok;

  while(cur) {
    fprintf(stderr, "%s\n (%p)", TokenKindName[cur->kind], cur->str);
    cur = cur->next;
  }
}
