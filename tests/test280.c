// EXPECT: 50
// SECTION: グローバル変数とローカル変数の名前衝突

int counter;
int scope_inc_global(){
    counter = counter + 1;
    return counter;
}
int main(){
    counter = 0;
    int counter = 999;
    scope_inc_global();
    scope_inc_global();
    scope_inc_global();
    return counter - 949;
}

