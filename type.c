#include"mcc.h"
#define INT_SIZE 8

//
// めっちゃマンパワーゾーンだった。
void add_type(Node *node){
    if (node == NULL) return;
    if (node->type != NULL) return;

    add_type(node->lhs);
    add_type(node->rhs);

    switch (node->kind){
        case ND_ADD:
            // &a + 1; とか片方がND_NUMの時は、コンパイル時に&aがintのポインタ型であれば1*4とかに変換できる。
            // TODO: 一旦シンプルに。charとか増やした時単なるINT or PTRだけの処理ではだめ。
            if (node->lhs->type->kind == TY_INT &&  node->rhs->type->kind == TY_INT){
                Type *type = calloc(1, sizeof(Type));
                type->kind = TY_INT;
                type->size = INT_SIZE;
                node->type = type;
            }

            if (node->lhs->type->kind == TY_PTR && node->rhs->type->kind == TY_INT){
                // 型を受け継ぐ
                Type *head = calloc(1, sizeof(Type));
                Type *cur = head;
                cur->kind = TY_PTR;
                cur->to_ptr = calloc(1, sizeof(Type));
                cur = cur->to_ptr;
                cur->size = INT_SIZE;
                cur->kind = TY_INT;
                node->type = head;

                // &x + 1 といったポインタ変数　+ NUMの計算になる。
                Node *tmp = node->rhs;
                node->rhs = new_node(ND_MUL, tmp, new_node_num(INT_SIZE));
            }

            if (node->lhs->type->kind == TY_INT && node->rhs->type->kind == TY_PTR){
                // 型を受け継ぐ
                Type *head = calloc(1, sizeof(Type));
                Type *cur = head;
                cur->kind = TY_PTR;
                cur->to_ptr = calloc(1, sizeof(Type));
                cur = cur->to_ptr;
                cur->size = INT_SIZE;
                cur->kind = TY_INT;
                node->type = head;

                // &x + 1 といったポインタ変数　+ NUMの計算になる。
                Node *tmp = node->lhs;
                node->lhs = new_node(ND_MUL, tmp, new_node_num(INT_SIZE));
            }
            break;

        case ND_SUB:
            break;
        case ND_MUL:
            break;
        case ND_DIV:
            break;
        case ND_EQ:
            break;
        case ND_NE:
            break;
        case ND_LT:
            break;
        case ND_LE:
            break;
        case ND_GT:
            break;
        case ND_GE:
            break;
        case ND_ASSIGN:
            break;
        case ND_LVAR:
            break;
        case ND_RETURN:
            break;
        case ND_IF:
            break;
        case ND_WHILE:
            break;
        case ND_FOR:
            break;
        case ND_BLOCK:
            break;
        case ND_FUNCALL:
            break;
        case ND_FUNCDEF:
            break;
        case ND_ADDR:
            // TODO:ここにもなんか実装する必要がある気がする
            // &a
            // ND_ADDR->ND_LVAL
            Type *lvar_type = node->lhs->type;
            Type *head = calloc(1, sizeof(Type));

            head->to_ptr = lvar_type;
            head->kind = TY_PTR;
            node->type = head;
            return;
            break;
        case ND_DEREF:
            break;
        case ND_NUM:
            break;
    }

}
