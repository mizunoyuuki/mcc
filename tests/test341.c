// EXPECT: 0
// SECTION: for文初期化での構造体ポインタ変数定義
struct A { int a; };
int main(){
    for (struct A *p; 0; ) {
    }
    return 0;
}
