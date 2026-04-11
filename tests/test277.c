// EXPECT: 13
// SECTION: グローバル変数とローカル変数の名前衝突

int x;
int scope_read_global(){
    return x;
}
int main(){
    x = 10;
    int x = 3;
    return x + scope_read_global();
}

