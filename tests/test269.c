// EXPECT: 12
// SECTION: 関数ごとのスコープ（同名ローカル変数）

int scope_add_one(int x){
    return x + 1;
}
int main(){
    int x = 11;
    return scope_add_one(x);
}

