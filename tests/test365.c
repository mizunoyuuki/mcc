// EXPECT: 8
// SECTION: continue はネストの内側ループのみに作用する
int main() {
    int i;
    int j;
    int x;
    x = 0;
    for (i = 0; i < 2; i = i + 1) {
        for (j = 0; j < 5; j = j + 1) {
            if (j == 2) continue;
            x = x + 1;
        }
    }
    return x;
}
