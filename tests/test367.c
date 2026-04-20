// EXPECT: 5
// SECTION: for + continue でインクリメント式が実行される
int main() {
    int i;
    int x;
    x = 0;
    for (i = 0; i < 10; i = i + 1) {
        if (i < 5) continue;
        x = x + 1;
    }
    return x;
}
