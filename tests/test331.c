// EXPECT: 8
// SECTION: 構造体変数に対するsizeof(アラインメントによるパディング)
struct A { char c; int a; };
int main(){
    struct A x;
    return sizeof(x);
}
