// EXPECT: 99
// SECTION: ポインタ型返り値の関数

  int *get_ptr(int *p){ return p; }
  int main(){
      int a = 0;
      *get_ptr(&a) = 99;
      return a;
  }
  
