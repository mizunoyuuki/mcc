// EXPECT: 1
// SECTION: グローバル変数（複合テスト）

int g;
int main(){
    g = 42;
    int *p = &g;
    if (*p == 42) return 1;
    return 0;
}

