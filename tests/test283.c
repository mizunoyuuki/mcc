// EXPECT: 33
// SECTION: グローバル変数とローカル変数の名前衝突

int x;
int scope_use_ten(){
    int x = 10;
    return x;
}
int scope_use_twenty(){
    int x = 20;
    return x;
}
int main(){
    x = 3;
    return scope_use_ten() + scope_use_twenty() + x;
}

