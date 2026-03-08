// 現状の加減乗除演算 | 比較演算 プログラムの文法
// expr       = equaloity
// equality   = relational ("==" relational | "!=" relational)*
// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
// add        = mul ("+" mul | "-" mul)*
// mul        = unary ("*" unary | "/" unary)*
// unary      = ("+" | "-")? primary
// primary    = NUM | "(" expr ")"

#include<stdio.h>
#include"mcc.h"

Token *tokenize(char *);
Node *expr(void);
void gen(Node *);

// 現在のtokenの外部変数
Token *token;

int main(int argc, char *argv[]){
        if (argc != 2){
                error("引数の個数が正しくありません。");
                return 1;
        }

        token = tokenize(argv[1]);
        Node *node = expr();

        printf(".intel_syntax noprefix\n");
        printf(".globl main\n");
        printf("main: \n");

        gen(node);

        printf("    pop rax\n");
        printf("    ret\n");

        return 0;
}
