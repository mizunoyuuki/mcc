// EXPECT: 0
// SECTION: 構造体変数宣言(他の変数と共存)
struct A { int a; int b; }; int main(){ int x; struct A y; return 0; }
