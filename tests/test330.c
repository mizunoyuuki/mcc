// EXPECT: 8
// SECTION: 構造体変数に対するsizeof(メンバ2つ)
struct A { int a; int b; };
int main(){
    struct A x;
    return sizeof(x);
}
