// EXPECT: 10
// SECTION: break後にループ外の処理が続く
int main() {
    int i;
    int x;
    x = 0;
    for (i = 0; i < 100; i = i + 1) {
        if (i == 5) break;
        x = x + 2;
    }
    return x;
}
