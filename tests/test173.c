// EXPECT: 100
// SECTION: グローバル変数（複合テスト）

int g;
int main(){
    g = 0;
    int *p = &g;
    while (*p < 100){
        *p = *p + 5;
    }
    return g;
}

