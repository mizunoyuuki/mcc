// EXPECT: 20
// SECTION: グローバル変数（複合テスト）

int g;
int main(){
    g = 0;
    int *p = &g;
    for (int i = 0; i < 5; i = i + 1)
        *p = *p + 4;
    return g;
}

