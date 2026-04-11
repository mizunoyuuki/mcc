// EXPECT: 99
// SECTION: グローバル変数（複合テスト）

int g;
int main(){
    g = 0;
    int *p = &g;
    *p = 99;
    return g;
}

