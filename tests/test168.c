// EXPECT: 30
// SECTION: グローバル変数（複合テスト）

int g;
int main(){
    g = 10;
    int l = 20;
    int *pg = &g;
    int *pl = &l;
    return *pg + *pl;
}

