// EXPECT: 55
// SECTION: グローバル変数（複合テスト）

int g;
int read_ptr(int *p){
    return *p;
}
int main(){
    g = 55;
    return read_ptr(&g);
}

