// EXPECT: 7
// SECTION: 構造体ポインタを挟んだ後のint変数が正しいか
struct A { int a; int b; };
int main(){
    int x = 3;
    struct A *p;
    int y = 7;
    return y;
}
