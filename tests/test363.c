// EXPECT: 4
// SECTION: while + continue：特定の値をスキップ
int main() {
    int i;
    int x;
    i = 0;
    x = 0;
    while (i < 5) {
        i = i + 1;
        if (i == 3) continue;
        x = x + 1;
    }
    return x;
}
