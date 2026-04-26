// EXPECT: 42
// SECTION: typedef (複数のエイリアス定義)
typedef int IntMy;
typedef char Byte;
int main(){ IntMy a = 40; Byte b = 2; return a + b; }
