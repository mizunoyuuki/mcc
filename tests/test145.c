// EXPECT: 42
// SECTION: グローバル変数

int g;
int set_g(){
    g = 42;
    return 0;
}
int main(){
    set_g();
    return g;
}

