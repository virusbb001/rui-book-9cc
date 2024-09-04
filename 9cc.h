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
} NodeKind;

typedef struct Node Node;

// 抽象構文木のノードの型
struct Node {
  NodeKind kind; // ノードの型
  Node *lhs;     // 左辺
  Node *rhs;     // 右辺
  int val;       // kindがND_NUMの場合のみ使う
};

Token *tokenize(char *p);
Node *expr();

void gen(Node *node);

// 現在着目しているトークン
extern Token *token;
// 入力プログラム
extern char *user_input;

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
