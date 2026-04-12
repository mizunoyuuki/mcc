// EXPECT: 0
// SECTION: 構造体ポインタと非ポインタの共存
struct A { int a; };
int main(){
    struct A x;
    struct A *p;
    return 0;
}
