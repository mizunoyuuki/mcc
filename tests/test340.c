// EXPECT: 8
// SECTION: 構造体ポインタのsizeof(構造体サイズと無関係に8)
struct A { int a; int b; int c; };
int main(){
    struct A *p;
    return sizeof(p);
}
