// EXPECT: 0
// SECTION: continue後の文は実行されない
int main() {
    int i;
    int x;
    i = 0;
    x = 0;
    while (i < 3) {
        i = i + 1;
        continue;
        x = 99;
    }
    return x;
}
