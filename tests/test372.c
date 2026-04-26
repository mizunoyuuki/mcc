// EXPECT: 8
// SECTION: typedef (関数の引数に使う)
typedef int MyInt;
MyInt add(MyInt a, MyInt b){ return a + b; }
int main(){ return add(3, 5); }
