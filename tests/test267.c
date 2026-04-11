// EXPECT: 15
// SECTION: 関数ごとのスコープ（同名ローカル変数）

int scope_sub(){
    int x = 100;
    return x;
}
int main(){
    int x = 15;
    scope_sub();
    return x;
}

