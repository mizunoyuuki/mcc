// EXPECT: 10
// SECTION: ブロックスコープ（外側参照 / スタック再利用 / ネスト）

int main(){
    int a = 1;
    {
        int b = 2;
        {
            int c = 3;
            {
                int d = 4;
                return a + b + c + d;
            }
        }
    }
    return 0;
}

