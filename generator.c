#include"mcc.h"
void gen(Node *node);

void gen_lval(Node *node){
    if (node->kind == ND_DEREF){
        gen(node->lhs);
        return;
    }
	if (node->kind != ND_LVAR)
		error ("代入の左辺値が変数, デリファレンスポインタではありません");

	printf("    mov rax, rbp\n");
	printf("    sub rax, %d\n", node->offset);
	printf("    push rax\n");
}

static int label_count = 0;

char *farg_registers_64[] = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
char *farg_registers_32[] = {"edi", "esi", "edx", "ecx", "r8d", "r9d"};
char *farg_registers_8[]  = {"dil", "sil", "dl",  "cl",  "r8b", "r9b"};

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

void gen (Node *node){
	switch (node->kind){
		case ND_NUM:
			printf("    push %d\n", node->val);
			return;
		case ND_LVAR:
			gen_lval(node);

			printf("    pop rax\n");
            // raxの先で変数が保存していたデータがchar 1バイト int 4バイトとかでメモリから読み取る命令が変化する
            if (node->type->size == 8) {
                printf("    mov rax, [rax]\n");
            } else if (node->type->size == 4) {
                printf("    movsxd rax, dword ptr [rax]\n");
            } else if (node->type->size == 1) {
                printf("    movsx rax, byte ptr [rax]\n");
            }
			printf("    push rax\n");
			return;

		case ND_ASSIGN:
			gen_lval(node->lhs);
			gen(node->rhs);

			printf("    pop rdi\n");
			printf("    pop rax\n");

            // 左辺の型のサイズ分を格納する命令に書き換える
            int s = node->lhs->type->size;
            if (s == 8) {
                printf("    mov [rax], rdi\n");
            } else if (s == 4) {
                printf("    mov dword ptr [rax], edi\n");
            } else if (s == 1) {
                printf("    mov byte ptr [rax], dil\n");
            }
			printf("    push rdi\n");
			return;
		case ND_RETURN:
			gen(node->lhs);
			printf("    pop rax\n");
			printf("    mov rsp, rbp\n");
			printf("    pop rbp\n");
			printf("    ret\n");
			return;
		case ND_IF:
			int label_if = label_count++;
			gen(node->cond);
			printf("    pop rax\n");
			printf("    cmp rax, 0\n");
			if(node->els){
				// else説がある場合
				printf("    je .Lelse%d\n", label_if);
				gen(node->then);
				printf("    pop rax\n");
				printf("    jmp .Lend%d\n", label_if);
				printf(".Lelse%d:\n", label_if);
				gen(node->els);
				printf("    pop rax\n");
				printf(".Lend%d:\n", label_if);
			} else {
				printf("    je .Lend%d\n", label_if);
				gen(node->then);
				printf("    pop rax\n");
				printf(".Lend%d:\n", label_if);
			}
			printf("    push 0\n");
			return;

		case ND_WHILE:
			int label_while = label_count++;
			printf(".Lbegin%d:\n", label_while);
			gen(node->lhs);
			printf("    pop rax\n");
			printf("    cmp rax, 0\n");
			printf("    je .Lend%d\n", label_while);
			gen(node->rhs);
			printf("    pop rax\n");
			printf("    jmp .Lbegin%d\n", label_while);
			printf(".Lend%d:\n", label_while);
			printf("    push 0\n");
			return;

		case ND_FOR:
			int label_for = label_count++;
			if (node->finit){
				gen(node->finit);
				printf("    pop rax\n");
			}
			printf(".Lbegin%d:\n", label_for);

			if (node->fcond){
				gen(node->fcond);
				printf("    pop rax\n");
				printf("    cmp rax, 0\n");
				printf("    je .Lend%d\n", label_for);
			}

			gen(node->fthen);
			printf("    pop rax\n");

			if(node->finc){
				gen(node->finc);
				printf("    pop rax\n");
			}

			printf("    jmp .Lbegin%d\n", label_for);
			printf(".Lend%d:\n", label_for);
			printf("    push 0\n");

			return;

		case ND_BLOCK:
			for (Node *n = node->body; n; n = n->next){
				gen(n);
				printf("    pop rax\n");
			}
			printf("    push 0\n");
			return;

		case ND_FUNCALL:
			int i = 0;
			for (Node *n = node->farg_body; n; n = n->next_farg){
				gen(n);
                printf("    pop %s\n", farg_registers_64[i++]);
			}

			printf("    mov rax, rsp\n");
			printf("    and rax, 15\n");
			printf("    jnz .Lcall%d\n", label_count);
			printf("    mov rax, 0\n");
			printf("    call %.*s\n", node->funclen, node->funcname);
			printf("    jmp .Lend%d\n", label_count);
			printf(".Lcall%d:\n", label_count);
                        printf("    sub rsp, 8\n");
                        printf("    mov rax, 0\n");
                        printf("    call %.*s\n", node->funclen, node->funcname);
                        printf("    add rsp, 8\n");
			printf(".Lend%d:\n", label_count);

			label_count++;
			printf("    push rax\n");

			return;

		case ND_FUNCDEF:
			// ラベル
			printf(".globl %.*s\n", node->funclen, node->funcname);
			printf("%.*s:\n", node->funclen, node->funcname);
			//
			// プロローグ
                        // 変数26個数分の領域を確保する
                        //
                        printf("    push rbp\n");
                        printf("    mov rbp, rsp\n");
                        printf("    sub rsp, 208\n");

			int arg_i = 0;
			for (Node *n = node->farg_body; n; n = n->next_farg){
                if (n->type->size == 8) {
                    printf("    mov [rbp-%d], %s\n", n->offset, farg_registers_64[arg_i++]);
                } else if (n->type->size == 4) {
                    printf("    mov [rbp-%d], %s\n", n->offset, farg_registers_32[arg_i++]);
                } else if (n->type->size == 1) {
                    printf("    mov [rbp-%d], %s\n", n->offset, farg_registers_8[arg_i++]);
                }
			}

			// 本体
			for (Node *n = node->func_body; n; n = n->next_func_stmt){
				gen(n);
				printf("    pop rax\n");
			}

			// エピローグ
			printf("    mov rsp, rbp\n");
			printf("    pop rbp\n");
			printf("    ret\n");

			return;

		case ND_ADDR:
			// lhsにunary()のノードが入ってる。
			// ND_ADDRの時、lhsにはidentが入ってる。
			// そのidentのアドレスをpushする
			gen_lval(node->lhs);
			return;

		case ND_DEREF:
			// lhsにunary()のノードが入ってる。
			// *aとか、*10かをコンパイルできるようにする
			gen(node->lhs);
			printf("    pop rax\n");
            if (node->type->size == 8) {
                printf("    mov rax, [rax]\n");
            } else if (node->type->size == 4) {
                printf("    movsxd rax, dword ptr [rax]\n");
            } else if (node->type->size == 1) {
                printf("    movsx rax, byte ptr [rax]\n");
            }
            printf("    push rax\n");
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
			printf("    movzb rax, al\n");
			break;
		case ND_NE:
			printf("    cmp rax, rdi\n");
			printf("    setne al\n");
			printf("    movzb rax, al\n");
			break;
		case ND_LT:
			printf("    cmp rax, rdi\n");
			printf("    setl al\n");
			printf("    movzb rax, al\n");
			break;
		case ND_LE:
			printf("    cmp rax, rdi\n");
			printf("    setle al\n");
			printf("    movzb rax, al\n");
			break;
		case ND_GT:
			printf("    cmp rax, rdi\n");
			printf("    setg al\n");
			printf("    movzb rax, al\n");
			break;
		case ND_GE:
			printf("    cmp rax, rdi\n");
			printf("    setge al\n");
			printf("    movzb rax, al\n");
			break;

	}

	printf("    push rax\n");
}


