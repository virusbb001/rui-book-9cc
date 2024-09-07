#include "tokenize.h"

// 抽象構文木のノードの種類
typedef enum {
  ND_NOT_SET, // 誤の防止
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
  ND_IF,
  ND_WHILE,
  ND_FOR,
  ND_BLOCK,
  ND_CALL, // 関数呼び出し
  ND_RETURN, // return
} NodeKind;

extern const char* NodeKindName[ND_RETURN+1];

typedef struct Node Node;
typedef struct NodeList NodeList;

// 抽象構文木のノードの型
struct Node {
  NodeKind kind; // ノードの型
  Node *lhs;     // 左辺
  Node *rhs;     // 右辺
  
  // 制御構文用

  Node *body; // 制御構文の中身
  Node *expr; // if, while, forの2つ目
  Node *init; // for の初期化式
  Node *update; // forの更新式
  Node *elseStmt; // elseの文

  /**
  * ND_BLOCK用 {} の中身
  * 関数呼び出しだったら引き数
  */
  NodeList *nodeList;

  int val;       // kindがND_NUMの場合のみ使う
  int offset;   // kindがND_LVARの場合のみ使う
  
  // ND_CALL用
  char *name; // 変数名
  int len; // 名前の長さ
};

// Nodeの連結リスト
struct NodeList {
  Node *car;
  NodeList *cdr;
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
