#include"mcc.h"

// 外部変数
// これに、Cの言語レベルでのベース型の配列情報を作っておく。
TypeRegistry *type_registry;

// type_registryの連結リストの初期化
// ベース型を先に登録しておく
void initialize_type_registry(){
    // int型
    TypeRegistry *cur_type_regi = calloc(1, TypeRegistry);
    cur_type_regi->name = "int";
    cur_type_regi->name_len = 3;
    Type *type = calloc(1, sizeof(Type));
    type->kind = TY_INT;
    type->size = 4;
    cur_type_line->type = type;
    type_registry =  cur_type_regi;

    // char型
    cur_type_regi = calloc(1, TypeRegistry);
    cur_type_regi->name = "char";
    cur_type_regi->name_len = 4;
    Type *type = calloc(1, sizeof(Type));
    type->kind = TY_CHAR;
    type->size = 1;
    cur_type_line->type = type;
    type_registry->next = curt_type_regi;
}
