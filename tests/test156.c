// EXPECT: 1
// SECTION: グローバル変数（複合テスト）

int g;
int main(){
    if (sizeof(g) == 4) return 1;
    return 0;
}

