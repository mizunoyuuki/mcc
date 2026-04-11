// EXPECT: 55
// SECTION: 関数ごとのスコープ（同名ローカル変数）

int scope_fib(int n){
    if (n == 0) return 0;
    if (n == 1) return 1;
    return scope_fib(n - 1) + scope_fib(n - 2);
}
int main(){
    return scope_fib(10);
}

