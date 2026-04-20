// EXPECT: 4
// SECTION: for + continue：特定の値をスキップ
int main() {
    int i;
    int x;
    x = 0;
    for (i = 0; i < 5; i = i + 1) {
        if (i == 3) continue;
        x = x + 1;
    }
    return x;
}
