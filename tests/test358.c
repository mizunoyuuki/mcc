// EXPECT: 1
// SECTION: while + break の基本動作
int main() {
    int i;
    i = 0;
    while (1) {
        i = i + 1;
        break;
    }
    return i;
}
