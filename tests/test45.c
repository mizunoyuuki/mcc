// EXPECT: 20
// SECTION: 関数定義（引数を使用）

int double_it(int x){
    int y = x + x;
    return y;
}
int main(){ return double_it(10); }

