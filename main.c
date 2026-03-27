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
void program(void);
void add_type(Node*);
Node* optimize(Node*);

// 現在のtokenの外部変数
Token *token;
Node *code[100];
LVar *locals;
TypeSpecifier type_specifiers[] = {
    {"int",  TK_INT_TYPE,  3, 8},
    {"char", TK_CHAR_TYPE, 4, 8},
};

int main(int argc, char *argv[]){

    if (argc != 2){
            error("引数の個数が正しくありません。");
            return 1;
    }

    // 字句解析: Tokenリスト作成
    token = tokenize(argv[1]);
    // 構文解析: AST作成
	program();
    // 意味解析: 全てのノードに型を付与
    for(int i = 0; code[i]; i++){
        add_type(code[i]);
    }

    // 最適化フェーズ
    for(int i = 0; code[i]; i++){
        code[i] = optimize(code[i]);
    }

    printf(".intel_syntax noprefix\n");
	
	//先頭の式から
	for (int i=0; code[i]; i++){
        // アセンブリ生成
		gen(code[i]);

		// 式の評価結果としてスタックに一つの値が残っている
		// はずなので、スタックが煽れないようにpopしておく
	}
    return 0;
}
