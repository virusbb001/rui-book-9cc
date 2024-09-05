#include "tokenize.h"

// 抽象構文木のノードの種類
typedef enum {
  ND_ADD, // +
  ND_SUB, // -
  ND_MUL, // *
  ND_DIV, // /
  ND_NUM, // 整数
  ND_EQ,  // ==
  ND_NE,  // !=
  ND_LT,  // <
  ND_LEQ, // <=
  ND_GT,  // >
  ND_GEQ, // >=
  ND_ASSIGN, // =
  ND_LVAR, // ローカル変数
  ND_RETURN, // return
} NodeKind;

typedef struct Node Node;

// 抽象構文木のノードの型
struct Node {
  NodeKind kind; // ノードの型
  Node *lhs;     // 左辺
  Node *rhs;     // 右辺
  int val;       // kindがND_NUMの場合のみ使う
  int offset;   // kindがND_LVARの場合のみ使う
};

Token *tokenize(char *p);
Node *expr();
void program();

void gen(Node *node);

// 現在着目しているトークン
extern Token *token;
// 入力プログラム
extern char *user_input;
// 
extern Node *code[100];

typedef struct LVar LVar;

// ローカル変数の型
struct LVar {
  LVar *next; // 次の変数かNULL
  char *name; // 変数の名前
  int len;    // 名前の長さ
  int offset; // RBPからのオフセット
};

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
