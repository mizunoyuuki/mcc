// EXPECT: 33
// SECTION: グローバル変数（複合テスト）

char g;
int main(){
    g = 33;
    char *p = &g;
    return *p;
}

