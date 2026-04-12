// EXPECT: 0
// SECTION: 構造体ポインタの後にint変数
struct A { int a; int b; };
int main(){
    struct A *p;
    int x;
    return 0;
}
