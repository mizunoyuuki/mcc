// EXPECT: 0
// SECTION: break後の文は実行されない
int main() {
    int i;
    i = 0;
    while (1) {
        break;
        i = 99;
    }
    return i;
}
