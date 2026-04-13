// EXPECT: 55
// SECTION: 構造体メンバをforループで使う
struct S {
    int sum;
    int i;
};
int main(){
    struct S s;
    s.sum = 0;
    for (s.i = 1; s.i <= 10; s.i = s.i + 1) {
        s.sum = s.sum + s.i;
    }
    return s.sum;
}
