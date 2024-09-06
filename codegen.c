#include <stdio.h>
#include "9cc.h"
#include "errors.h"

int labelIndex = 0;

void gen_lval(Node *node) {
  if (node->kind != ND_LVAR)
    error("代入の左辺値が変数ではありません");

  printf("    mov rax, rbp\n");
  printf("    sub rax, %d\n", node->offset);
  printf("    push rax\n");
}

void gen(Node *node) {
  switch(node->kind) {
    case ND_RETURN:
      gen(node->lhs);
      printf("    pop rax\n");
      printf("    mov rsp, rbp\n");
      printf("    pop rbp\n");
      printf("    ret\n");
      return;
    case ND_NUM:
      printf("    push %d\n", node->val);
      return;
    case ND_LVAR:
      gen_lval(node);
      printf("    pop rax\n");
      printf("    mov rax, [rax]\n");
      printf("    push rax\n");
      return;
    case ND_ASSIGN:
      gen_lval(node->lhs);
      gen(node->rhs);

      printf("    pop rdi\n");
      printf("    pop rax\n");
      printf("    mov [rax], rdi\n");
      printf("    push rdi\n");
      return;
    case ND_IF:
      gen(node->expr);
      printf("    pop rax\n");
      printf("    cmp rax, 0\n");
      if(node->elseStmt) {
        printf("    je .Lelse%03d\n", labelIndex);
        gen(node->body);
        printf("    jmp .Lend%03d\n", labelIndex);
        printf(".Lelse%03d:\n", labelIndex);
        gen(node->elseStmt);
      } else {
        printf("    je .Lend%03d\n", labelIndex);
        gen(node->body);
      }
      printf(".Lend%03d:\n", labelIndex);
      labelIndex++;
      return;
    case ND_WHILE:
      printf(".Lbegin%03d:\n", labelIndex);
      gen(node->expr);
      printf("    pop rax\n");
      printf("    cmp rax, 0\n");
      printf("    je .Lend%03d\n", labelIndex);
      gen(node->body);
      printf("    jmp .Lbegin%03d\n", labelIndex);
      printf(".Lend%03d:\n", labelIndex);
      labelIndex++;
      return;
    case ND_FOR:
      gen(node->init);
      printf(".Lbegin%03d:\n", labelIndex);
      gen(node->expr);
      printf("    pop rax\n");
      printf("    cmp rax, 0\n");
      printf("    je .Lend%03d\n", labelIndex);
      gen(node->body);
      gen(node->update);
      printf("    jmp .Lbegin%03d\n", labelIndex);
      printf(".Lend%03d:\n", labelIndex);
      labelIndex++;
      return;
    case ND_BLOCK:
      {
        NodeList *current = node->nodeList;
        while(current->car) {
          gen(current->car);
          printf("    pop rax\n");
          current = current->cdr;
        }
      }
      return;
    // 2項式は次でまとめて処理する
    case ND_ADD:
    case ND_SUB:
    case ND_MUL:
    case ND_DIV:
    case ND_EQ:
    case ND_NE:
    case ND_LT:
    case ND_LEQ:
    case ND_GT:
    case ND_GEQ:
      break;
    default:
      error("gen Not implemented: %d", node->kind);
      return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("    pop rdi\n");
  printf("    pop rax\n");
  switch(node->kind) {
    case ND_ADD:
      printf("    add rax, rdi\n");
      break;
    case ND_SUB:
      printf("    sub rax, rdi\n");
      break;
    case ND_MUL:
      printf("    imul rax, rdi\n");
      break;
    case ND_DIV:
      printf("    cqo\n");
      printf("    idiv rdi\n");
      break;
    case ND_EQ:
      printf("    cmp rax, rdi\n");
      printf("    sete al\n");
      printf("    movzb rax,al\n");
      break;
    case ND_NE:
      printf("    cmp rax, rdi\n");
      printf("    setne al\n");
      printf("    movzb rax,al\n");
      break;
    case ND_LT:
      printf("    cmp rax, rdi\n");
      printf("    setl al\n");
      printf("    movzb rax,al\n");
      break;
    case ND_LEQ:
      printf("    cmp rax, rdi\n");
      printf("    setle al\n");
      printf("    movzb rax,al\n");
      break;
    case ND_GT:
      printf("    cmp rdi, rax\n");
      printf("    setl al\n");
      printf("    movzb rax,al\n");
      break;
    case ND_GEQ:
      printf("    cmp rdi, rax\n");
      printf("    setle al\n");
      printf("    movzb rax,al\n");
      break;
    default:
      error("not should pass");
  }

  printf("    push rax\n");
}

