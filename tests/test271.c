// EXPECT: 15
// SECTION: 関数ごとのスコープ（同名ローカル変数）

int scope_sum(int n){
    if (n == 0) return 0;
    return n + scope_sum(n - 1);
}
int main(){
    return scope_sum(5);
}

