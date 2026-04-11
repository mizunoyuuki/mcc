// EXPECT: 10
// SECTION: グローバル変数（複合テスト）

int g;
int count_up(){
    if (g >= 10) return g;
    g = g + 1;
    return count_up();
}
int main(){
    g = 0;
    return count_up();
}

