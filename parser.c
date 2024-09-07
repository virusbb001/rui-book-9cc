#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "9cc.h"


Node *code[100];
// ローカル変数
LVar *locals;

// 変数を名前で検索する。見つからなかた場合はNULLを返す。
LVar *find_lvar(Token *tok) {
  for (LVar *var = locals; var; var = var->next) {
    if (var->len == tok->len && !memcmp(tok->str, var->name, var->len))
      return var;
  }
  return NULL;
}

/**
 * @param tok 確認したいトークン
 * @param op 記号
  */
bool is_token_reserved(Token *tok, char *op) {
  return tok->kind == TK_RESERVED
  && strlen(op) == tok->len
  && memcmp(tok->str, op, tok->len) == 0
  ;
}

// 次のトークンが期待している記号のときには、トークンを1つ読み進めて
// 真を返す。それ以外の場合には偽を返す。
bool consume(char *op) {
  bool matched = is_token_reserved(token, op);
  if (!matched)
    return false;
  token = token->next;
  return true;
}

Token* consume_special(TokenKind kind) {
  if (token->kind == kind) {
    Token* cur = token;
    token = token->next;
    return cur;
  }
  return NULL;
}

Token *consume_ident() {
  if (token->kind == TK_IDENT) {
    Token *cur = token;
    token = token->next;
    return cur;
  }
  return NULL;
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

Node *assign() {
  Node *node = equality();
  if (consume("="))
    node = new_node(ND_ASSIGN, node, assign());
  return node;
}

Node *expr() {
  return assign();
}

Node *stmt() {
  Node *node;

  if (consume_special(TK_IF)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_IF;
    expect("(");
    node->expr = expr();
    expect(")");
    node->body = stmt();
    if (consume_special(TK_ELSE)) {
      node->elseStmt = stmt();
    }
    return node;
  }

  if (consume_special(TK_WHILE)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_WHILE;
    expect("(");
    node->expr = expr();
    expect(")");
    node->body = stmt();
    return node;
  }

  if (consume_special(TK_FOR)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_FOR;
    expect("(");
    if (!consume(";")) {
      node->init=expr();
      expect(";");
    }
    if (!consume(";")) {
      node->expr=expr();
      expect(";");
    }
    if (!consume(")")) {
      node->update=expr();
      expect(")");
    }
    node->body = stmt();
    return node;
  }

  if (consume("{")) {
    Node *node = calloc(1, sizeof(Node));
    NodeList *current = calloc(1, sizeof(NodeList));
    node->kind = ND_BLOCK;
    node->nodeList = current;
    while(!consume("}")) {
      current->car = stmt();
      current->cdr = calloc(1, sizeof(NodeList));
      current = current->cdr;
    }
    return node;
  }

  if (consume_special(TK_RETURN)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_RETURN;
    node->lhs = expr();
  } else {
    node = expr();
  }
  expect(";");
  return node;
}

void program() {
  int i=0;
  while (!at_eof())
    code[i++] = stmt();
  code[i] = NULL;
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

  Token *tok = consume_ident();
  if (tok) {
    Node *node = calloc(1, sizeof(Node));
    if (consume("(")) {
      node->kind = ND_CALL;
      node->nodeList = calloc(1, sizeof(NodeList));
      node->name = tok->str;
      node->len = tok->len;
      NodeList *current = node->nodeList;
      while (!consume(")")) {
        current->car = expr();
        current->cdr = calloc(1, sizeof(NodeList));
        current = current->cdr;
        consume(",");
      }
      return node;
    }

    node->kind = ND_LVAR;

    LVar *lvar = find_lvar(tok);
    if (lvar) {
      node->offset = lvar->offset;
    } else {
      lvar = calloc(1, sizeof(LVar));
      lvar->next = locals;
      lvar->name = tok->str;
      lvar->len = tok->len;
      lvar->offset = (locals == NULL ? 0 : locals->offset) + 8;
      node->offset = lvar->offset;
      locals = lvar;
    }
    return node;
  }

  // そうでなければ数値のはず
  return new_node_num(expect_number());
}

