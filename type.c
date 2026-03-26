#include"mcc.h"

void add_type(Node *node){
    if (node == NULL) return;
    if (node->type != NULL) return;

    add_type(node->lhs);
    add_type(node->rhs);

    switch (node->kind){
        case ND_ADD:
            if (node->lhs->type == node->lhs->type)
                node->type = node->lhs->type;
        case ND_SUB:
        case ND_MUL:
        case ND_DIV:
        case ND_EQ:
        case ND_NE:
        case ND_LT:
        case ND_LE:
        case ND_GT:
        case ND_GE:
        case ND_ASSIGN:
        case ND_LVAR:
        case ND_RETURN:
        case ND_IF:
        case ND_WHILE:
        case ND_FOR:
        case ND_BLOCK:
        case ND_FUNCALL:
        case ND_FUNCDEF:
        case ND_ADDR:
        case ND_DEREF:
        case ND_NUM:
    }

    return;
}
