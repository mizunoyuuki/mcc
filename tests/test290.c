// EXPECT: 5
// SECTION: ブロックスコープ（外側参照 / スタック再利用 / ネスト）

int main(){
    int i = 0;
    while (i < 5) {
        int tmp = 1;
        i = i + tmp;
    }
    return i;
}

