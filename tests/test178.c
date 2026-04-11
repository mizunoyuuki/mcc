// EXPECT: 10
// SECTION: ポインタ型グローバル変数

  int val;
  int *gp;
  int main(){
      val = 10;
      gp = &val;
      return *gp;
  }
  
