// EXPECT: 10
// SECTION: グローバル変数（複合テスト）

int g;
int main(){
    g = 10;
    int *p = &g;
    int **pp = &p;
    return **pp;
}

