// EXPECT: 100
// SECTION: 構造体変数宣言(構造体の後にint変数)
struct A { int a; };
int main(){
    struct A x;
    int y;
    y = 100;
    if (y){
        int y = 5;
    }
    return y;
}
