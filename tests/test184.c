// EXPECT: 30
// SECTION: ポインタ型グローバル変数

  int *gpa;
  int *gpb;
  int main(){
      int a = 10;
      int b = 20;
      gpa = &a;
      gpb = &b;
      return *gpa + *gpb;
  }
  
