#include"mcc.h"
#define INT_SIZE 8

//
// めっちゃマンパワーゾーンだった。
void add_type(Node *node){
    if (node == NULL) return;
    if (node->type != NULL) return;

    switch (node->kind){
          case ND_IF:
              add_type(node->cond);
              add_type(node->then);
              add_type(node->els);
              break;
          case ND_FOR:
              add_type(node->finit);
              add_type(node->fcond);
              add_type(node->finc);
              add_type(node->fthen);
              break;
          case ND_BLOCK:
              for (Node *n = node->body; n; n = n->next)
                  add_type(n);
              break;
          case ND_FUNCDEF:
              for (Node *n = node->farg_body; n; n = n->next_farg)
                  add_type(n);
              for (Node *n = node->func_body; n; n = n->next_func_stmt)
                  add_type(n);
              break;
          case ND_FUNCALL:
              for (Node *n = node->farg_body; n; n = n->next_farg)
                  add_type(n);
              break;
          default:
              break;
    }

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

                return;
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

                return;
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

                return;
            }
            break;

        case ND_SUB:
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

            break;

        case ND_MUL:
            if (node->lhs->type->kind == TY_INT &&  node->rhs->type->kind == TY_INT){
                Type *type = calloc(1,sizeof(Type));
                type->kind = TY_INT;
                type->size = INT_SIZE;
                node->type = type;
            }
            break;
        case ND_DIV:
            if (node->lhs->type->kind == TY_INT &&  node->rhs->type->kind == TY_INT){
                Type *type = calloc(1,sizeof(Type));
                type->kind = TY_INT;
                type->size = INT_SIZE;
                node->type = type;
            }
            break;

        case ND_EQ:
        case ND_NE:
        case ND_LT:
        case ND_LE:
        case ND_GT:
        case ND_GE:
            Type *type = calloc(1,sizeof(Type));
            type->kind = TY_INT;
            type->size = INT_SIZE;

            node->type = type;

            break;
        case ND_ASSIGN:
            break;
        case ND_LVAR:
            break;
        case ND_RETURN:
            break;
        case ND_WHILE:
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
            node->type = node->lhs->type->to_ptr;
            break;
        case ND_NUM:
            break;
        case ND_FUNCALL:
            Type *t = calloc(1, sizeof(Type));
            t->kind = TY_INT;
            t->size = INT_SIZE;
            node->type = t;
        default:
            break;
    }

}
