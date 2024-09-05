#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "tokenize.h"
#include "errors.h"

int is_alnum(char c) {
  return ('a' <= c && c <= 'z') ||
    ('A' <= c && c <= 'Z') ||
    ('0' <= c && c <= '9') ||
    (c == '_');
}

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

    if (strncmp(p, "return", 6) == 0 && !is_alnum(p[6])) {
      cur = new_token(TK_RETURN, cur, p, 6);
      p += 6;
      continue;
    }

    if ('a' <= *p && *p <= 'z') {
      char *start = p;
      while(('a' <= *p && *p <= 'z')) {
        p = p+1;
      }
      cur = new_token(TK_IDENT, cur, start, p-start);
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

    if (
      *p == '+'
      || *p == '-'
      || *p == '*'
      || *p == '/'
      || *p == '('
      || *p == ')'
      || *p == ';'
      || *p == '='
    ) {
      cur = new_token(TK_RESERVED, cur, p++, 1);
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p, 1);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error("トークナイズできません: %c", *p);
  }

  new_token(TK_EOF, cur, p, 1);
  return head.next;
}
