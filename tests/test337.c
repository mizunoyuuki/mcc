// EXPECT: 3
// SECTION: int変数の後に構造体ポインタ、その後にint変数
struct A { int a; int b; };
int main(){
    int x = 3;
    struct A *p;
    int y = 7;
    return x;
}
