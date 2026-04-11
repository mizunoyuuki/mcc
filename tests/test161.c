// EXPECT: 50
// SECTION: グローバル変数（複合テスト）

int g;
int main(){
    g = 0;
    int *p = &g;
    while (g < 50)
        *p = *p + 10;
    return g;
}

