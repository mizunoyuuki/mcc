// EXPECT: 5
// SECTION: 構造体ポインタの後のint変数に値を代入
struct A { int a; };
int main(){
    struct A *p;
    int x = 5;
    return x;
}
