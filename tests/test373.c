// EXPECT: 99
// SECTION: typedef (グローバル変数に使う)
typedef int MyInt;
MyInt g;
int main(){ g = 99; return g; }
