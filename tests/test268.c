// EXPECT: 6
// SECTION: 関数ごとのスコープ（同名ローカル変数）

int scope_f(){
    int a = 1;
    int b = 2;
    return a + b;
}
int scope_g(){
    int a = 10;
    int b = 20;
    return a - b + 20;
}
int main(){
    int a = 100;
    int b = 200;
    return scope_f() + scope_g() - 7;
}

