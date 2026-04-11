// EXPECT: 5
// SECTION: ブロックスコープ（外側参照 / スタック再利用 / ネスト）

int main(){
    if (0) {
        int a = 3;
        return a;
    } else {
        int a = 5;
        return a;
    }
    return 0;
}

