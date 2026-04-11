// EXPECT: 7
// SECTION: グローバル変数とローカル変数の名前衝突

int n;
int scope_add_to_arg(int n){
    return n + 5;
}
int main(){
    n = 100;
    return scope_add_to_arg(2);
}

