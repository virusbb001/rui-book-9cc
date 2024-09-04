#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>

#include "tokenize.h"
#include "errors.h"

// 新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  tok->len = len;
  cur->next = tok;
  return tok;
}

// 入力文字列pをトークナイズしてそれを返す
Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    // 空白文字をスキップ
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (*p == '=' && *(p+1) == '=') {
      cur = new_token(TK_RESERVED, cur, p, 2);
      p = p+2;
      continue;
    }

    if (*p == '!' && *(p+1) == '=') {
      cur = new_token(TK_RESERVED, cur, p, 2);
      p = p+2;
      continue;
    }

    if (*p == '<') {
      if (*(p+1) == '=') {
        cur = new_token(TK_RESERVED, cur, p, 2);
        p = p+2;
      } else {
        cur = new_token(TK_RESERVED, cur, p++, 1);
      }
    }

    if (*p == '>') {
      if (*(p+1) == '=') {
        cur = new_token(TK_RESERVED, cur, p, 2);
        p = p+2;
      } else {
        cur = new_token(TK_RESERVED, cur, p++, 1);
      }
    }

    if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')') {
      cur = new_token(TK_RESERVED, cur, p++, 1);
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p, 1);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error("トークナイズできません");
  }

  new_token(TK_EOF, cur, p, 1);
  return head.next;
}
