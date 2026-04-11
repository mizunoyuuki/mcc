// EXPECT: 42
// SECTION: ポインタ型グローバル変数

  int *gp;
  int main(){
      int a = 42;
      gp = &a;
      return *gp;
  }
  
