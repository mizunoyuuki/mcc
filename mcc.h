#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<ctype.h>
#include<stdarg.h>
#include<string.h>

#define PTR_SIZE 8
#define INT_SIZE 4
#define CHAR_SIZE 1

// tokenの種類
typedef enum {
	TK_RESERVED,  // 記号
	TK_IDENT,     // 識別子
	TK_RETURN,    // returnキーワード
        TK_IF,        // if
	TK_ELSE,      // else
	TK_WHILE,     // while
        TK_FOR,       // for
	TK_NUM,       // 整数トークン
        TK_INT_TYPE,  // int型
        TK_CHAR_TYPE, // char型
	TK_SIZEOF,    // sizeof演算子
	TK_EOF        // 入力の終わりを表すトークン
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

// C言語の言語仕様としてサポートしている型の配列。
// tokenizerで、文字=>トークンに変換するために使う
typedef struct TypeSpecifier {
    char *type_name;
    TokenKind token_kind;
    int len;
} TypeSpecifier;

typedef enum {
	TY_INT,
	TY_CHAR,
	TY_PTR,
} TypeKind;

typedef struct Type Type;

struct Type {
	TypeKind kind;
	Type *to_ptr;
	int size;
};

// パーサーで使うような型情報配列。
typedef struct TypeRegistry TypeRegistry;

struct TypeRegistry{
	char *name;
	int name_len;
	Type *type;
	TypeRegistry *next;
};

// Node型のNodeKindの列挙型
typedef enum {
	ND_ADD,      // +
	ND_SUB,      // -
	ND_MUL,      // *
	ND_DIV,      // /
	ND_EQ,       // ==
	ND_NE,       // !=
	ND_LT,       // <
	ND_LE,       // <=
	ND_GT,       // >
	ND_GE,       // >=
	ND_ASSIGN,   // =(代入)
	ND_LVAR,     // ローカル変
        ND_RETURN,   // return
        ND_IF,       // if
	ND_WHILE,    // while
	ND_FOR,      // for
	ND_BLOCK,    // {} ブロック
	ND_FUNCALL,  // 関数呼び出し
	ND_FUNCDEF,  // 関数の定義
	ND_ADDR,     // アドレス参照
	ND_DEREF,    // アドレスデリファレンス
	ND_SIZEOF,   // sizeof演算子
	ND_NUM,      // 整数
} NodeKind;

// Node型
typedef struct Node Node;
typedef struct LVar LVar;

struct Node {
	NodeKind kind; // ノードの型
	Node *lhs;     // 左辺
	Node *rhs;     // 右辺
        
	// if文 Node
	Node *cond;   // ifの条件式
	Node *then;    // ifのthen節
	Node *els;     // ifのelse節
		       //
		       //
        // for文 Node
	Node *finit;    // 初期化(expr?)
	Node *fcond;    // 条件式(expr?)
	Node *finc;     // 更新式(expr?)
	Node *fthen;    // ループの中身(stmt)
		
	// Block(複文) / Node構造体を連結リストとして使えるように追加
	Node *next;
	Node *body;

	// 関数呼び出し
	char *funcname;
	int funclen;
	// 関数引数(定義、呼び出し)
	// 関数呼び出され時に、どの値をレジスタからスタックに戻せばいいのかを認識するため
	Node *farg_body; 
	Node *next_farg;
	// 関数内のローカル変数を管理する構造体連結リスト
	LVar *func_lvar;

	// 関数本体(定義)
	Node *func_body;
	Node *next_func_stmt;

	// 型情報
	Type *type;


	// 単項演算子のtypeofの子ノード
	Node *sizeof_target;
	int  sizeof_val;

	int val;       // kindがND_NUMの場合のみ扱う
        int offset;    // kindがND_LVARの場合のみ使う
};

// LVar型 ローカル変数の型
struct LVar {
	LVar *next;   // 次の変数かNULL
	char *name;   // 変数名
	int len;      // 変数の長さ
	Type *type;   // 変数型を表す連結リスト
	int offset;   // RBPからアクセスするためのオフセット値
};


// 外部変数の宣言
extern Token *token;
extern Node *code[100];
extern LVar *locals;
extern TypeSpecifier type_specifiers[2];
extern TypeRegistry *type_registry;

// type.cで、parse.cで定義した関数を使いたくなる。他にもありそうなので、optimizeでも使うと思う.
extern Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
extern Node *new_node_num(int val);

extern void error(char*, ...);
