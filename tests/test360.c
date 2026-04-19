// EXPECT: 3
// SECTION: break はネストの内側ループのみ抜ける
int main() {
    int i;
    int j;
    for (i = 0; i < 3; i = i + 1) {
        for (j = 0; j < 10; j = j + 1) {
            break;
        }
    }
    return i;
}
