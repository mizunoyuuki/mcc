// EXPECT: 15
// SECTION: 構造体メンバを条件式で使う
struct S {
    int x;
};
int main(){
    struct S s;
    s.x = 10;
    if (s.x == 10) {
        s.x = 15;
    }
    return s.x;
}
