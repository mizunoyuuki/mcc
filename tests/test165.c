// EXPECT: 88
// SECTION: グローバル変数（複合テスト）

int g;
int main(){
    g = 0;
    int *p = &g;
    int **pp = &p;
    **pp = 88;
    return g;
}

