// EXPECT: 99
// SECTION: 関数引数にポインタ型

  int set_val(int *p){
      *p = 99;
      return 0;
  }
  int main(){
      int a = 1;
      set_val(&a);
      return a;
  }
  
