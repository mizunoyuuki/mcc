// EXPECT: 4
// SECTION: 構造体変数に対するsizeof(メンバ1つ)
struct A { int a; };
int main(){
    struct A x;
    return sizeof(x);
}
