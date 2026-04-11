// EXPECT: 100
// SECTION: 関数定義（複合テスト）

int compute(){
    int a = 0;
    for (int i = 0; i < 10; i = i + 1)
        a = a + 10;
    return a;
}

int main(){
    return compute();
}

