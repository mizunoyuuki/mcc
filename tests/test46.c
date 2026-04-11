// EXPECT: 55
// SECTION: 関数定義（引数を使用）

int fib(int n){
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}
int main(){ return fib(10); }

