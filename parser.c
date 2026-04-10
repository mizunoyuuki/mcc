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
Node *top();
Node *globl_var(Token*, Token*, Type*);
Node *funcdef(Token*, Type*);
Node *stmt();
Node *expr();
Node *assign();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *primary();
Node *unary();
Node *parse_globl_declaration(Token*, Token*, Type*);
Node *parse_declaration(void);
Token *type_keyword();
bool consume(char*);
void expect(char*);
int expect_number();
bool at_eof();
GVar *find_gvar(Token *);
LVar *find_lvar(Token *);
FuncEntry *find_func(Token *);

TypeRegistry *find_type_registry(Token*);

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
		code[i++] = top();
	}

	code[i] = NULL;
}

// fucdef        = ident "(" params? ")" "{" stmt* "}"
// params        = ident ( "," ident )*
//

// topレベルでのparser関数
Node *top(){
    locals = NULL;

    Token *type_token = type_keyword();

    if (!type_token){
        error("型の定義がありません,");
    }

    token = token->next;

    // *の数だけTypeの連結リストを作る
    Type head;
    head.to_ptr = NULL;
    Type *cur = &head;
    while (consume("*")){
        Type *t = calloc(1, sizeof(Type));
        t->kind = TY_PTR;
        t->size = PTR_SIZE;
        cur->to_ptr = t;
        cur = cur->to_ptr;
    }

    // 新しく作った型でも認識して続ける。
    TypeRegistry *ty_regi = find_type_registry(type_token);

	if (!ty_regi){
		error("型がありません。");
	}

    // identityトークンをを取得する
    Token *token_ident = consume_ident();

    // 配列型だった場合
    Type *last_t = calloc(1, sizeof(Type));
    last_t->kind = ty_regi->type->kind;
    last_t->size = ty_regi->type->size;
    cur->to_ptr = last_t;

    if (consume("[")){
        Type *arr = calloc(1, sizeof(Type));
        int index = expect_number();

        arr->array_size = index;
        arr->size = index * ty_regi->type->size;
        arr->kind = TY_ARRAY;
        expect("]");

        arr->to_ptr = last_t;
        head.to_ptr = arr;
    }

    
    // 関数の場合
    if (consume("(")){
        // トークン、Type
        return funcdef(token_ident, head.to_ptr);
    // グローバル変数の場合
    } else {
        // トークン, Type
        return globl_var(type_token, token_ident, head.to_ptr);
    }
}

// int a;
Node *globl_var(Token *type_token, Token *indent_token, Type *type){
    Node *gval = parse_globl_declaration(type_token, indent_token, type);
    return gval;
}

Node *funcdef(Token *tok, Type *type){
	locals = NULL;
	Node *node = calloc(1, sizeof(Node));

	node->kind = ND_FUNCDEF;
	node->funcname = tok->str;
	node->funclen = tok->len;
    node->type = type;


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

    FuncEntry *fe = func_entry;
    FuncEntry *cur_f = calloc(1, sizeof(FuncEntry));
    cur_f->name = tok->str;
    cur_f->type = type;
    cur_f->next = fe;
    func_entry = cur_f;

	return node;
}

Node *stmt(){
	Node *node;

	// 変数宣言: type ident ("=" expr)? ";"
    // ポインタ変数としての定義もできるように
	if (type_keyword()){
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
			if (type_keyword()){
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

    if (token->kind == TK_CHAR_CONST){
        Node *node = new_node_num(token->val);
        token = token->next;
        return node;
    }

    if (token->kind == TK_STRING){
        static int str_index = 0;

        // str_literals連結リストの作成
        StrLiteral *str = calloc(1, sizeof(StrLiteral));
        str->simbol_index = str_index;
        str->len = token->len;
        str->str = token->str;

        str->next = str_literals;
        str_literals = str;

        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_STRING;
        node->str_lite = str;
        token = token->next;

        // インクリメント
        str_index++;
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
            // 関数名をidentトークンから探す
            FuncEntry *fnc = find_func(ident_tok);
            if(fnc){ 
                node->type = fnc->type;
            }

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

        // local変数の確認 => global変数の確認の順番でヒットする変数を確認し、globl偏数の場合は、特別な型を作る。
		// 変数参照の場合
        // TODO: ローカル変数の場合と、グローバル変数の場合で、対応が変わる
        LVar *lvar = find_lvar(ident_tok);
        GVar *gvar = find_gvar(ident_tok);

        if (lvar){ 
            node->kind = ND_LVAR;
            node->offset = lvar->offset;
            node->type   = lvar->type;
            node->is_array = (lvar->type->kind == TY_ARRAY);
            // 配列のa[10]とかのアクセスの場合。
            if (consume("[")){
                Node *index = expr();
                expect("]");
                return new_node(ND_DEREF, new_node(ND_ADD, node, index), NULL);
            }
        } else if (gvar){
            node->kind = ND_GVAR;
            node->gvar_name = gvar->name;
            node->gvar_len  = gvar->len;
            node->type = gvar->type;
            node->is_array = (gvar->type->kind == TY_ARRAY);
            if (consume("[")){
                Node *index = expr();
                expect("]");
                return new_node(ND_DEREF, new_node(ND_ADD, node, index), NULL);
            }
        } else {
            error("未定義の変数です。");
        }

		return node;
	}

	return new_node_num(expect_number());
}

// 変数を名前で検索する。見つからなかったらNULLを返す。
GVar *find_gvar(Token *tok){
    for (GVar *var = globls; var; var=var->next){
        if (var->len == tok->len && !memcmp(tok->str, var->name, var->len))
            return var;
    }
    return NULL;
}

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
Token *type_keyword(){
    if (token->kind == TK_INT_TYPE || token->kind == TK_CHAR_TYPE ){
        return token;
    } else {
        return NULL;
    }
}



// 変数宣言をパースする: type ident ("=" expr)?
// セミコロンは呼び出し側で処理する
// 外部変数の管理する連結リストとローカル変数の連結リストは分ける
Node *parse_globl_declaration(Token *type_tok, Token *ident_tok, Type *type){
    GVar *gvar = find_gvar(ident_tok);

    if (gvar){
        error("グローバル変数が二重で定義されています。");
    }

    GVar *cur_globls = globls;
    gvar = calloc(1, sizeof(GVar));

    // グローバル変数の登録
    gvar->name = ident_tok->str;
    gvar->next = globls;
    gvar->len = ident_tok->len;
    globls = gvar;

    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_GVAR;
    // ローカル変数はNodeに登録したoffsetでアクセスするが、外部変数は名前をシンボルにアクセスする。
    node->gvar_name = gvar->name;
    node->gvar_len = gvar->len;

    gvar->type = type;
    node->type = type;
    node->is_array = (gvar->type->kind == TY_ARRAY);

    expect(";");
        
    return node;
}

Node *parse_declaration(){
    TypeKind ident_type = token->kind == TK_INT_TYPE ? TY_INT : TY_CHAR;
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

	Token *ident_tok = consume_ident();
	if (!ident_tok){
		error("変数名がありません。");
	}

	// 二重定義チェック
	LVar *lvar = find_lvar(ident_tok);
	if (lvar){
		error("変数が二重に定義されています。");
	}

    // 配列の場合と、普通の変数の場合でサイズをかえる
    if (consume("[")){
        int size;
        if (ident_type == TY_INT){
            size = 4;
        } else if (ident_type == TY_CHAR){
            size = 1;
        }

        int index = token->val;
        if (!index){
            error("配列の要素を指定してください");
        }
        // 数字を読んだのでトークンを進める.
        token = token->next;

        cur_type->size = size;
        Type *array_type = calloc(1, sizeof(Type));
        array_type->kind = TY_ARRAY;
        array_type->to_ptr = cur_type;
        array_type->array_size = size;
        array_type->size = size * index;
        head_type = array_type;

        expect("]");
    } else {
        if (ident_type == TY_INT){
            cur_type->size = 4;
        } else if (ident_type == TY_CHAR){
            cur_type->size = 1;
        }
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
    // 配列型の場合は、is_arrayにtrueを入れるようにする。
    if (head_type->kind == TY_ARRAY) {
        node->is_array = true;
    }
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

TypeRegistry *find_type_registry(Token *tk){
    for(TypeRegistry *cur = type_registry; cur; cur = cur->next){
        if (memcmp(tk->str, cur->name, tk->len) == 0)
            return cur;
    }
    return NULL;
}

FuncEntry *find_func(Token *tk){
    for(FuncEntry *f = func_entry; f; f = f->next){
        if (memcmp(tk->str, f->name, tk->len) == 0)
            return f;
    }

    return NULL;
}

// sizeof演算子でしか使わない。
int consume_type_size(){

    TokenKind tk = token->kind;
    if (tk != TK_INT_TYPE && tk != TK_CHAR_TYPE)
        return -1;

    // tokenの名前から、TypeRegisterを探しに行こうか。
    TypeRegistry *tr = find_type_registry(token);
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
