// EXPECT: 15
// SECTION: ブロックスコープ（外側参照 / スタック再利用 / ネスト）

int main(){
    int r = 0;
    if (1) {
        int x = 7;
        r = r + x;
    }
    if (1) {
        int y = 8;
        r = r + y;
    }
    return r;
}

