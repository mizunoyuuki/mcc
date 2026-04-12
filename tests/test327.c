// EXPECT: 3
// SECTION: for文の構造体変数とint変数の共存
struct A { int a; };
int main(){
    int x = 3;
    for (struct A y; 0; ) {
    }
    return x;
}
