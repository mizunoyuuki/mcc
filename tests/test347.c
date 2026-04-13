// EXPECT: 99
// SECTION: 構造体ポインタのアロー演算子（->）
struct A {
    int x;
};
int main(){
    struct A a;
    struct A *p;
    p = &a;
    p->x = 99;
    return a.x;
}
