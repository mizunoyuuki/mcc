// EXPECT: 30
// SECTION: 関数ごとのスコープ（同名ローカル変数）

int scope_a(){
    int x = 10;
    return x;
}
int scope_b(){
    int x = 20;
    return x;
}
int main(){
    return scope_a() + scope_b();
}

