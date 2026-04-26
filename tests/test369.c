// EXPECT: 10
// SECTION: typedef (int エイリアスで演算)
typedef int MyInt;
int main(){ MyInt x = 3; MyInt y = 7; return x + y; }
