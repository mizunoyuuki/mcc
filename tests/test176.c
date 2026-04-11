// EXPECT: 99
// SECTION: ポインタ型グローバル変数

  int *gp;
  int main(){
      int a = 0;
      gp = &a;
      *gp = 99;
      return a;
  }
  
