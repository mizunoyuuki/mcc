// EXPECT: 42
// SECTION: 関数ごとのスコープ（同名ローカル変数）

int scope_double_it(int n){
    n = n * 2;
    return n;
}
int main(){
    int n = 21;
    scope_double_it(n);
    return n + 21;
}

