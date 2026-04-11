// EXPECT: 77
// SECTION: ポインタ型グローバル変数

  int *gp;
  int set_gp(int *p){
      gp = p;
      return 0;
  }
  int main(){
      int a = 77;
      set_gp(&a);
      return *gp;
  }
  
