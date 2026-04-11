// EXPECT: 3
// SECTION: ポインタ型グローバル変数

  int **gpp;
  int main(){
      int a = 3;
      int *p = &a;
      gpp = &p;
      return **gpp;
  }
  
