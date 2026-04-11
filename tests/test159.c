// EXPECT: 77
// SECTION: グローバル変数（複合テスト）

int g;
int write_ptr(int *p){
    *p = 77;
    return 0;
}
int main(){
    g = 0;
    write_ptr(&g);
    return g;
}

