// EXPECT: 42
// SECTION: 構造体メンバへの代入と参照（.演算子）
struct A {
    int x;
};
int main(){
    struct A a;
    a.x = 42;
    return a.x;
}
