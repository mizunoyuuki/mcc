// EXPECT: 100
// SECTION: グローバル変数とローカル変数の名前衝突

int x;
int scope_get_global_x(){
    return x;
}
int main(){
    x = 100;
    int x = 7;
    x = 8;
    return scope_get_global_x();
}

