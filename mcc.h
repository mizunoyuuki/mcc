#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<ctype.h>
#include<stdarg.h>
#include<string.h>

// tokenの種類
typedef enum {
	TK_RESERVED,
	TK_NUM,
	TK_EOF
} TokenKind;


// Token型の定義
// 演算子が文字列の場合もあるため、lenを追加する
typedef struct Token Token;
struct Token {
	TokenKind kind;
        Token *next;
	int val;
	char *str;
	int len;
};

typedef enum {
	ND_ADD,  // +
	ND_SUB,  // -
	ND_MUL,  // *
	ND_DIV,  // /
	ND_EQ,   // ==
	ND_NE,   // !=
	ND_LT,   // <
	ND_LE,   // <=
	ND_GT,   // >
	ND_GE,   // >=
	ND_NUM,  // 整数
} NodeKind;

typedef struct Node Node;

struct Node {
	NodeKind kind; // ノードの型
	Node *lhs;     // 左辺
	Node *rhs;     // 右辺
	int val;       // kindがND_NUMの場合のみ扱う
};

extern Token *token;

extern void error(char*, ...);
