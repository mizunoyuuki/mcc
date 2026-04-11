// EXPECT: 20
// SECTION: ポインタ型グローバル変数

  int *gp;
  int main(){
      int a[3];
      *a = 10;
      *(a + 1) = 20;
      *(a + 2) = 30;
      gp = a + 1;
      return *gp;
  }
  
