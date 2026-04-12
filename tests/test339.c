// EXPECT: 12
// SECTION: 構造体変数と構造体ポインタのsizeofの差
struct A { int a; int b; int c; };
int main(){
    struct A x;
    struct A *p;
    return sizeof(x);
}
