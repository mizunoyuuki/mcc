// EXPECT: 42
// SECTION: グローバル変数（複合テスト）

int g;
int main(){
    g = 42;
    int *p = &g;
    return *p;
}

