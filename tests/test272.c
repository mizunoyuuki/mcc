// EXPECT: 24
// SECTION: 関数ごとのスコープ（同名ローカル変数）

int scope_fact(int n){
    int r;
    if (n == 0) return 1;
    r = n * scope_fact(n - 1);
    return r;
}
int main(){
    return scope_fact(4);
}

