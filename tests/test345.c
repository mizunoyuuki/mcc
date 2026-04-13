// EXPECT: 3
// SECTION: 構造体メンバへの再代入
struct S {
    int x;
};
int main(){
    struct S s;
    s.x = 1;
    s.x = 3;
    return s.x;
}
