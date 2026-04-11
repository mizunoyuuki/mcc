// EXPECT: 42
// SECTION: ブロックスコープ（外側参照 / スタック再利用 / ネスト）

int main(){
    int a = 42;
    if (1) {
        return a;
    }
    return 0;
}

