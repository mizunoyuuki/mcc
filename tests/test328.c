// EXPECT: 10
// SECTION: for文の構造体変数がスコープ外に漏れないこと(シャドーイング)
struct A { int a; };
int main(){
    int x = 10;
    for (struct A x; 0; ) {
    }
    return x;
}
