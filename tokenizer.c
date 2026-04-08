#include"mcc.h"
// 新しいトークンを作成してcurに繋げる
Token *new_token(TokenKind kind, Token *cur, char *str, int len){
        Token *tok = calloc(1, sizeof(Token));
        tok->kind = kind;
        tok->str = str;
        tok->len = len;
        cur->next = tok;
        return tok;
}

bool startswith(char *p, char *q){
        return memcmp(p, q, strlen(q)) == 0;
}

bool is_alphabet(char p){
	return (p >= 'a' && p <= 'z') || (p >= 'A' && p <= 'Z') || p == '_';
}

bool is_return(char *p){
	return !memcmp("return", p, 6) && (*(p+6) == ' ');
}

bool is_if(char *p){
	return !memcmp(p, "if", 2) && ( (*(p+2) == ' ') || (*(p+2) == '(') );
}

bool is_else(char *p){
	return !memcmp(p, "else", 4) && !is_alphabet(*(p+4));
}

bool is_while(char *p){
	return !memcmp(p, "while", 5) && !is_alphabet(*(p+5));
}

bool is_for(char *p){
	return !memcmp(p, "for", 3) && !is_alphabet(*(p+3));
}

bool is_sizeof(char *p){
    return !memcmp(p, "sizeof", 6) && !is_alphabet(*(p+6));
}

int is_char_const(char *p){
    if (*p == '\''){
        int start = (*(p+1) == '\\') ? 3 : 2;  // ← バックスラッシュなら3から
        for(int i = start;; i++){
            if (*(p + i) == '\''){
                return i-1;
            }
        }
    }
    return 0;
}

// C言語デフォルトの型をトークナイズするための関数
TypeSpecifier *is_type_specifier(char *p){
    for (int i = 0; i < sizeof(type_specifiers) / sizeof(type_specifiers[0]); i++)
        if (!memcmp(p, type_specifiers[i].type_name, type_specifiers[i].len)
            && !is_alphabet(*(p + type_specifiers[i].len)))
            return &type_specifiers[i];
    return NULL;
}

// 入力文字列pをトークナイズしてそれを返す。
// 現在の文法
//
// program       = funcdef*
// fucdef        = ident "(" params? ")" "{" stmt* "}"
// params        = ident ( "," ident )*
// stmt          = expr ";"
//               | "{" stmt* "}"
//               | "return" expr ";"
//               | "if" "(" expr ")" stmt ( "else" stmt )?
//               | "for" "(" expr? ";" expr? ";" expr? ")" stmt
//               | "while" "(" expr ")" stmt
// expr          = assign
// assign        = equaity ( "=" assign )?
// equality      = relational ( "==" relational | "!=" relational )*
// relational    = add ( "<" add | "<=" add | ">" add | ">=" add )*
// add           = mul ( "+" mul | "-" mul )*
// mul           = unary ( "*" unary | "/" unary )
// unary         = ("+" | "-")? primary
// primary       = num
//               | ident ( "(" (assign? ("," assign)*)? ")" )?
//               | "(" expr ")"
// 

Token *tokenize(char *p){
	Token head;
	head.next = NULL;
	Token *cur = &head;

	while (*p){
		if (isspace(*p)){
			p++;
			continue;
		}

        TypeSpecifier *type = is_type_specifier(p);
        if (type){
            cur = new_token(type->token_kind, cur, p, type->len);
            p += type->len;
            continue;
        }
		if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' || *p == ')' || *p == ';' || *p == ',' || *p == '&' ||  *p == '[' || *p == ']' ){
			cur = new_token(TK_RESERVED, cur, p++, 1);
			continue;
		}

		if (startswith(p, "==") || startswith(p, "!=") || startswith(p, "<=") || startswith(p, ">=") ){
			cur = new_token(TK_RESERVED, cur, p, 2);
			p+=2;
			continue;
		}

        int a = is_char_const(p);
        if(a){
            cur = new_token(TK_CHAR_CONST, cur, p, a);

            if (a == 1){
                cur->val = *(p + 1); // 通常文字
            } else {
                switch(*(p + 2)) {
                    case 'n': cur->val = '\n'; break;
                    case 't': cur->val = '\t'; break;
                    case 'r':  cur->val = '\r'; break;  // 13
                    case '0':  cur->val = '\0'; break;  // 0
                    case '\\': cur->val = '\\'; break;  // 92
                    case '\'': cur->val = '\''; break;  // 39
                    default:   error("不正なエスケープシーケンス");
                }
            }
            p = p + a + 2;
            continue;
        }

        if (is_sizeof(p)){
            cur = new_token(TK_SIZEOF, cur, p, 6);
            p += 6;
            continue;
        }

		if (*p == '<' || *p == '>' || *p == '='){
			cur = new_token(TK_RESERVED, cur, p++, 1);
			continue;
		}

		if (*p == '{' || *p == '}'){
			cur = new_token(TK_RESERVED, cur, p++, 1);
			continue;
		}

		if (is_if(p)){
			cur = new_token(TK_IF, cur, p, 2);
			p += 2;
			continue;
		}

		if (is_else(p)){
			cur = new_token(TK_ELSE, cur, p, 4);
			p += 4;
			continue;
		}

		if (is_while(p)){
			cur = new_token(TK_WHILE, cur, p, 5);
			p += 5;
			continue;
		}

		if (is_for(p)){
			cur = new_token(TK_FOR, cur, p, 3);
			p += 3;
			continue;
		}

		if (is_return(p)){
			cur = new_token(TK_RETURN, cur, p, 6);
			p += 6;
			continue;
		}

		if (is_alphabet(*p)){
			char *q = p;
			int len = 0;

	                while (is_alphabet(*p)){
				p++;
				len += 1;
			}

			cur = new_token(TK_IDENT, cur, q, len);
			continue;
		}

		if (isdigit(*p)){
			cur = new_token(TK_NUM, cur, p, 0);
			char *q = p;
			cur->val = strtol(p, &p, 10);
			cur->len = p-q;
			continue;
		}

		error("トークナイズできません。");
	}

	new_token(TK_EOF, cur, p, 0);
	return head.next;
}
