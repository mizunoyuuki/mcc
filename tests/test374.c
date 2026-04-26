// EXPECT: 3
// SECTION: typedef (エイリアスのポインタ)
typedef int MyInt;
int main(){ MyInt x = 3; MyInt *p = &x; return *p; }
