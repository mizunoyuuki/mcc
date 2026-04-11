// EXPECT: 9
// SECTION: ブロックスコープ（外側参照 / スタック再利用 / ネスト）

int main(){
    int a = 9;
    int i = 0;
    while (i < 1) {
        i = i + 1;
        return a;
    }
    return 0;
}

