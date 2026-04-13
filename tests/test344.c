// EXPECT: 5
// SECTION: 構造体メンバ同士の演算
struct S {
    int a;
    int b;
};
int main(){
    struct S s;
    s.a = 15;
    s.b = 10;
    return s.a - s.b;
}
