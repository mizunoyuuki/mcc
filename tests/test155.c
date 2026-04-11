// EXPECT: 8
// SECTION: グローバル変数（複合テスト）

int g;
int main(){
    return sizeof(g) + sizeof(g);
}

