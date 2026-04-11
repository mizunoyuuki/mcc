// EXPECT: 7
// SECTION: グローバル変数

int g;
int get_g(){ return g; }
int main(){
    g = 7;
    return get_g();
}

