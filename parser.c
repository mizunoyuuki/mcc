#include"mcc.h"

Node *new_node(NodeKind kind, Node *lhs, Node *rhs){
	Node *node = calloc(1, sizeof(Node));
	node->kind = kind;
	node->lhs = lhs;
	node->rhs = rhs;
	return node;
}

Node *new_node_num(int val){
	Node *node = calloc(1, sizeof(Node));
	node->kind = ND_NUM;
	node->val = val;
    Type *type = calloc(1, sizeof(Type));
    type->kind = TY_INT;
    type->size = INT_SIZE;
    node->type = type;

	return node;
}

// 現在の文法
// program       = funcdef*
// funcdef       = type ident "(" params? ")" "{" stmt* "}"
// params        = ident ( "," ident )*
// stmt          = type ident ("=" expr)? ";"
//               | expr ";"
//               | "{" stmt* "}"
//               | "return" expr ";"
//               | "if" "(" expr ")" stmt ( "else" stmt )?
//               | "for" "(" (type ident ("=" expr)? | expr)? ";" expr? ";" expr? ")" stmt
//               | "while" "(" expr ")" stmt
// expr          = assign
// assign        = equality ( "=" assign )?
// equality      = relational ( "==" relational | "!=" relational )*
// relational    = add ( "<" add | "<=" add | ">" add | ">=" add )*
// add           = mul ( "+" mul | "-" mul )*
// mul           = unary ( "*" unary | "/" unary )
// unary         = ("+" | "-")? primary
// primary       = num
//               | ident ( "(" (assign? ("," assign)*)? ")" )?
//               | "(" expr ")"
//

// 関数の宣言
Node *funcdef();
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *primary();
Node *unary();
Node *parse_declaration(void);
bool is_type_keyword();
bool consume(char*);
void expect(char*);
int expect_number();
bool at_eof();
LVar *find_lvar(Token *);

Token *consume_ident(void);
int consume_type_size(void);
bool consume_return(char *op);
bool consume_if(char *op);
bool consume_else(char *op);
bool consume_while(char *op);
bool consume_for(char *op);
bool consume_sizeof(char *op);

void program(){
	int i = 0;
	while (!at_eof()){
		code[i++] = funcdef();
	}

	code[i] = NULL;
}

// fucdef        = ident "(" params? ")" "{" stmt* "}"
// params        = ident ( "," ident )*

Node *funcdef(){
	locals = NULL;
	Node *node = calloc(1, sizeof(Node));
	if (!is_type_keyword()){
		error("型の定義がありません。");
	}
	token = token->next; // 型キーワードを消費

	Token *tok = consume_ident();
	if (!tok){
		error("関数名がありません。");
	}

	node->kind = ND_FUNCDEF;
	node->funcname = tok->str;
	node->funclen = tok->len;


	// 関数の引数部分
	// 引数はそのローカル変数が定義されているかのように書く
	// 関数の呼び出しとは違い、関数の定義は、assignではなく、変数宣言しかおけない。
	// LVar 構造体
	// struct LVar {
	//    LVar *next;
	//    char *name;
	//    int  len;
	//    int  offset;
	// }
	expect("(");
	if (!consume(")")){
		// 最初の引数
        Node *cur_arg = parse_declaration();
        Node *head_arg = cur_arg;

		while(consume(",")){
            Node *arg = parse_declaration();

            cur_arg->next_farg = arg;
            cur_arg = arg;
		}

		expect(")");
        node->farg_body = head_arg;
	}



	// 関数本体
	expect("{");
	node->func_body = stmt();
	Node *cur_func_stmt = node->func_body;
	while(!consume("}")){
		cur_func_stmt->next_func_stmt = stmt();
		cur_func_stmt = cur_func_stmt->next_func_stmt;
	}

	node->func_lvar = locals;
	return node;
}

Node *stmt(){
	Node *node;

	// 変数宣言: type ident ("=" expr)? ";"
    // ポインタ変数としての定義もできるように
	if (is_type_keyword()){
		node = parse_declaration();
		expect(";");
		return node;
	}

	if (consume_return("return")){
        node = calloc(1, sizeof(Node));
        node->kind = ND_RETURN;
		node->lhs = expr();
		expect(";");
		return node;
	}

	if (consume_if("if")){
		node = calloc(1, sizeof(Node));
		node->kind = ND_IF;
		expect("(");
		node->cond = expr();
		expect(")");
		node->then = stmt();
		if(consume_else("else")){
			node->els = stmt();
		}
		return node;
	}

	// node->blockは、Node型のポインタの配列となっている
	// Node **block;
	if (consume("{")){
		node = calloc(1, sizeof(Node));
		node->kind = ND_BLOCK;

		Node head;
		head.next = NULL;
		Node *cur = &head;

		while (!consume("}")){
			Node *block_statement = calloc(1, sizeof(Node));
			block_statement = stmt();
			cur->next = block_statement;
			cur = cur->next;
		}

		node->body = head.next;

		return node;
	}

	if (consume_while("while")){
		node = calloc(1, sizeof(Node));
		node->kind = ND_WHILE;
		expect("(");
		node->lhs = expr();
		expect(")");
		node->rhs = stmt();
		return node;
	}

	if (consume_for("for")){
		node = calloc(1, sizeof(Node));
		node->kind = ND_FOR;
		expect("(");
		if (!consume(";")){
			if (is_type_keyword()){
				node->finit = parse_declaration();
			} else {
				node->finit = expr();
			}
			expect(";");
		}

		if (!consume(";")){
			node->fcond = expr();
			expect(";");
		}

		if (!consume(")")){
			node->finc = expr();
			expect(")");
		}
		node->fthen = stmt();
		return node;
	}

	node = expr();
	expect(";");

	return node;
}


Node *expr(){
	return assign();
}

Node *assign(){
	Node *node = equality();

	if (consume("=")){
		node = new_node(ND_ASSIGN, node, assign());
	}
	return node;
}


Node *equality(){
	Node *node = relational();

	for(;;){
		if (consume("==")){
			node = new_node(ND_EQ, node, relational());
		} else if(consume("!=")){
			node = new_node(ND_NE, node, relational());
		} else {
			return node;
		}
	}
}

Node *relational(){
	Node *node = add();

	for (;;){
		if (consume("<"))
			node = new_node(ND_LT, node, add());
		else if (consume(">"))
			node = new_node(ND_GT, node, add());
		else if (consume("<="))
			node = new_node(ND_LE, node, add());
		else if (consume(">="))
			node = new_node(ND_GE, node, add());
		else
			return node;
	}
}

Node *add(){
	Node *node = mul();

	for(;;){
		if (consume("+"))
			node = new_node(ND_ADD, node, mul());
		else if (consume("-"))
			node = new_node(ND_SUB, node, mul());
		else
			return node;
	}
}


Node *mul(){
	Node *node = unary();

	for(;;){
		if (consume("*")){
			node = new_node(ND_MUL, node, unary());
		} else if (consume("/")) {
			node = new_node(ND_DIV, node, unary());
		} else {
			return node;
		}
	}
}

Node *unary(){
	if (consume("+"))
		return primary();
	if (consume("-"))
		return new_node(ND_SUB, new_node_num(0), primary());
	if (consume("*"))
		return new_node(ND_DEREF, unary(), NULL);
	if (consume("&"))
		return new_node(ND_ADDR, unary(), NULL);
    if (consume_sizeof("sizeof")){
        // sizeof(x);
        // のxを処理して、Typeがなんなのか、sizeがなんなのかを計算
        // TODO: とりあえず、sizeof(変数名)だけをサポートさせる

        consume("(");
        int t_si = consume_type_size();

        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_SIZEOF;

        if (t_si > 0){
            node->sizeof_val = t_si;
        } else {
            node->sizeof_target = expr();
        }

        expect(")");

        return node;
    }
	return primary();
}

Node *primary(){
	if (consume("(")){
		Node *node = expr();
		expect(")");
		return node;
	}


	Token *ident_tok = consume_ident();

	if (ident_tok){
		Node *node = calloc(1, sizeof(Node));
		// 関数呼び出しの場合
		if (consume("(")){
			node->kind = ND_FUNCALL;
			node->funcname = ident_tok->str;
			node->funclen = ident_tok->len;
			if (consume(")")) {
				return node;
			}

			node->farg_body = assign();
			Node *cur_farg = node->farg_body;

			while (consume(",")) {
				cur_farg->next_farg = assign();
				cur_farg = cur_farg->next_farg;
			}

			expect(")");
			return node;
		}

		// 変数参照の場合
		node->kind = ND_LVAR;

		LVar *lvar = find_lvar(ident_tok);
		if (!lvar){
			error("未定義の変数です。変数は型をつけて定義してください。");
		}
		node->offset = lvar->offset;
        node->type   = lvar->type;
		return node;
	}

	return new_node_num(expect_number());
}

// 変数を名前で検索する。見つからなかったらNULLを返す。
LVar *find_lvar(Token *tok){
	for (LVar *var = locals; var; var = var->next){
		if (var->len == tok->len &&  !memcmp(tok->str, var->name, var->len))
			return var;
	}

	return NULL;
}



// 次のトークンが期待している記号の時は、トークンを一つ読み進める。
// 真を返す。それ以外の場合は偽を返す。
bool consume(char *op){
	if (token->kind != TK_RESERVED || strlen(op) != token->len || memcmp(token->str, op, token->len))
		return false;
	token = token->next;
	return true;
}

bool consume_return(char *op){
	if (token->kind != TK_RETURN || strlen(op) != token->len || memcmp(token->str, op, token->len))
		return false;

	token = token->next;
	return true;
}

bool consume_if(char *op){
	if (token->kind != TK_IF || strlen(op) != token->len || memcmp(token->str, op, token->len))
		return false;
	token = token->next;
	return true;
}

bool consume_else(char *op){
	if (token->kind != TK_ELSE || strlen(op) != token->len || memcmp(token->str, op, token->len))
		return false;

	token = token->next;
	return true;
}

bool consume_while(char *op){
	if (token->kind != TK_WHILE || strlen(op) != token->len || memcmp(token->str, op, token->len))
		return false;
	token = token->next;
	return true;
}

bool consume_for(char *op){
	if (token->kind != TK_FOR || strlen(op) != token->len || memcmp(token->str, op, token->len)){
		return false;
	}

	token = token->next;
	return true;
}

bool consume_sizeof(char *op){
    if (token->kind != TK_SIZEOF || strlen(op) != token->len || memcmp(token->str, op, token->len))
        return false;
    token = token->next;
    return true;
}
// 次のトークンが期待している記号の時には、トークンを一つ読み進める。
// それ以外の場合にはエラーを報告する。
void expect(char *op){
	if (token->kind != TK_RESERVED || token->len != strlen(op) || memcmp(token->str, op, token->len))
		error("'%c'ではありません。", op);
	token = token->next;
}


// 次のトークンが数値の場合、トークンを一つ読み進めてその数値を返す
// それ以外の場合はエラーを報告する。
int expect_number() {
	if (token->kind != TK_NUM )
		error("数ではありません。");
	int val = token->val;
	token = token->next;
	return val;
}

// 現在のトークンが型キーワードかどうか（トークンは消費しない）
bool is_type_keyword(){
	return token->kind == TK_INT_TYPE || token->kind == TK_CHAR_TYPE;
}



// 変数宣言をパースする: type ident ("=" expr)?
// セミコロンは呼び出し側で処理する
Node *parse_declaration(){
    TokenKind ident_type = token->kind == TK_INT_TYPE ? TY_INT : TY_CHAR;
	token = token->next; // 型キーワードを消費

    // type指定子後に*があったらwhileで回してType型の連結リストを作っておく
    Type *head_type = calloc(1, sizeof(Type));
    Type *cur_type  = head_type;
    while (consume("*")){
        cur_type->kind = TY_PTR;
        cur_type->size = PTR_SIZE;
        cur_type->to_ptr = calloc(1, sizeof(Type));
        cur_type = cur_type->to_ptr;
    }
    // cur_type:ptr => cur_type:引数で受け取った
    // identのtypeの種類によって、Nodeに紐づけるType構造体のsizeを切り替える。int=4, char=1, ptr=8;
    cur_type->kind = ident_type;
    if (ident_type == TY_INT){
        cur_type->size = 4;
    } else if (ident_type == TY_CHAR){
        cur_type->size = 1;
    }

	Token *ident_tok = consume_ident();
	if (!ident_tok){
		error("変数名がありません。");
	}

	// 二重定義チェック
	LVar *lvar = find_lvar(ident_tok);
	if (lvar){
		error("変数が二重に定義されています。");
	}

	// localsに登録
	lvar = calloc(1, sizeof(LVar));
	lvar->next = locals;
	lvar->name = ident_tok->str;
	lvar->len = ident_tok->len;
    lvar->type = head_type;
    // ptrならhead_t
	lvar->offset = locals ? locals->offset + head_type->size : head_type->size;
	locals = lvar;

	// 初期化式があるか
	if (consume("=")){
		Node *lvar_node = calloc(1, sizeof(Node));
		lvar_node->kind = ND_LVAR;
		lvar_node->offset = lvar->offset;
        lvar_node->type = head_type;
		return new_node(ND_ASSIGN, lvar_node, expr());
	}

	// 初期化なし
	Node *node = calloc(1, sizeof(Node));
	node->kind = ND_LVAR;
	node->offset = lvar->offset;
    node->type = head_type;
	return node;
}

Token *consume_ident(){
    if (token->kind == TK_IDENT){
        Token *tok = token;
        token = token->next;
        return tok;
    }

    return NULL;

}

TypeRegistry *find_type_specifier(Token *tk){
    for(type_registry; type_registry; type_registry = type_registry->next ){
        if (memcmp(tk->str, type_registry->name, tk->len));
            return type_registry;
    }
    return NULL;
}

// sizeof演算子でしか使わない。
int consume_type_size(){

    TokenKind tk = token->kind;
    if (tk != TK_INT_TYPE && tk != TK_CHAR_TYPE)
        return -1;

    // tokenの名前から、TypeRegisterを探しに行こうか。🤔
    TypeRegistry *tr = find_type_specifier(token);
    token = token->next;

    // ポインタ型なら常に8バイト
    if (consume("*")){
        while (consume("*")) {}
        return PTR_SIZE;  // PTR_SIZE
    }

    return tr->type->size;
}


bool at_eof(){
	return token->kind == TK_EOF;
}
