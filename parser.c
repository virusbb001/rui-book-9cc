#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "9cc.h"


// 次のトークンが期待している記号のときには、トークンを1つ読み進めて
// 真を返す。それ以外の場合には偽を返す。
bool consume(char *op) {
  if (token->kind != TK_RESERVED ||
    strlen(op) != token->len ||
    memcmp(token->str, op, token->len))
    return false;
  token = token->next;
  return true;
}

// 次のトークンが期待している記号のときには、トークンを1つ読み進める。
// それ以外の場合にはエラーを報告する。
void expect(char *op) {
  if (token->kind != TK_RESERVED || 
    strlen(op) != token->len ||
    memcmp(token->str, op, token->len))
    error_at(token->str, "%sではありません", op);
  token = token->next;
}

// 次のトークンが数値の場合、トークンを1つ読み進めてその数値を返す。
// それ以外の場合にはエラーを報告する。
int expect_number() {
  if (token->kind != TK_NUM)
    error_at(token->str, "数ではありません");
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() {
  return token->kind == TK_EOF;
}

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

Node *mul();
Node *add ();
Node *primary();
Node *unary();
Node *equality();
Node *relational();

Node *expr() {
  return equality();
}

Node *equality() {
  Node *node = relational();

  for (;;) {
    if (consume("=="))
      node = new_node(ND_EQ, node, relational());
    if (consume("!="))
      node = new_node(ND_NE, node, relational());
    else
      return node;
  }
}

Node *relational () {
  Node *node = add();

  for (;;) {
    if (consume("<="))
      node = new_node(ND_LEQ, node, add());
    if (consume(">="))
      node = new_node(ND_GEQ, node, add());
    if (consume("<"))
      node = new_node(ND_LT, node, add());
    if (consume(">"))
      node = new_node(ND_GT, node, add());
    else
      return node;
  }
}

Node *add () {
  Node *node = mul();

  for(;;) {
    if (consume("+"))
      node = new_node(ND_ADD, node, mul());
    if (consume("-"))
      node = new_node(ND_SUB, node, mul());
    else
      return node;
  }
}

Node *mul() {
  Node *node = unary();

  for (;;) {
    if (consume("*"))
      node = new_node(ND_MUL, node, unary());
    if (consume("/"))
      node = new_node(ND_DIV, node, unary());
    else
      return node;
  }
}

Node *unary() {
  if (consume("+"))
    return primary();
  if (consume("-"))
    return new_node(ND_SUB, new_node_num(0), primary());
  return primary();
}

Node *primary() {
  // 次のトークンが"("なら、"(" expr ")"のはず
  if (consume("(")) {
    Node *node = expr();
    expect(")");
    return node;
  }

  // そうでなければ数値のはず
  return new_node_num(expect_number());
}

