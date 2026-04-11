// EXPECT: 55
// SECTION: ポインタ型グローバル変数

  int val;
  int *gp;
  int main(){
      val = 0;
      gp = &val;
      *gp = 55;
      return val;
  }
  
