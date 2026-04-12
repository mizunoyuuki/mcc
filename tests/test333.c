// EXPECT: 8
// SECTION: 構造体ポインタ変数のsizeof
struct A { int a; int b; };
int main(){
    struct A *p;
    return sizeof(p);
}
