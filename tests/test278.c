// EXPECT: 42
// SECTION: グローバル変数とローカル変数の名前衝突

int val;
int scope_set_global(){
    val = 40;
    return 0;
}
int scope_use_local(){
    int val = 2;
    return val;
}
int main(){
    scope_set_global();
    return val + scope_use_local();
}

