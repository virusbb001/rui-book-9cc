#ifndef TOKENIZE_H
#define TOKENIZE_H

// トークンの種類
typedef enum {
  TK_RESERVED, // 記号
  TK_IDENT,    // 記号
  TK_NUM,      // 整数トークン
  TK_RETURN,   // return
  TK_IF,       // if
  TK_ELSE,     // else
  TK_WHILE,    // while
  TK_FOR,      // for
  TK_EOF,      // 入力の終わりを表すトークン
} TokenKind;
typedef struct Token Token;

// トークン型
struct Token {
  TokenKind kind;   // トークンの型
  Token *next;      // 次の入力トークン
  int val;          // kindがTK_NUMの場合、その数値
  char *str;        // トークン文字列
  int len;          // トークンの長さ
};

#endif
