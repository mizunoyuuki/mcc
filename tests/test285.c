// EXPECT: 3
// SECTION: ブロックスコープ（外側参照 / スタック再利用 / ネスト）

int main(){
    int a = 3;
    {
        return a;
    }
    return 0;
}

