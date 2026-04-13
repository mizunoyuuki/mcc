// EXPECT: 7
// SECTION: 構造体メンバを使った複合演算
struct S {
    int a;
    int b;
    int c;
};
int main(){
    struct S s;
    s.a = 2;
    s.b = 3;
    s.c = 5;
    return s.a + s.c;
}
