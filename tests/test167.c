// EXPECT: 77
// SECTION: グローバル変数（複合テスト）

char g;
int main(){
    g = 0;
    char *p = &g;
    *p = 77;
    return g;
}

