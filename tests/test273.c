// EXPECT: 8
// SECTION: 関数ごとのスコープ（同名ローカル変数）

int scope_inc_five(){
    int count = 0;
    for (int i = 0; i < 5; i = i + 1)
        count = count + 1;
    return count;
}
int scope_inc_three(){
    int count = 0;
    for (int i = 0; i < 3; i = i + 1)
        count = count + 1;
    return count;
}
int main(){
    return scope_inc_five() + scope_inc_three();
}

