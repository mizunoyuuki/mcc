// EXPECT: 40
// SECTION: グローバル変数（複合テスト）

int g;
int main(){
    g = 0;
    if (sizeof(g) == 4){
        for (int i = 0; i < 10; i = i + 1)
            g = g + 4;
    }
    return g;
}

