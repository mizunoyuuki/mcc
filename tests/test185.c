// EXPECT: 42
// SECTION: ポインタ型返り値の関数

  int *get_ptr(int *p){ return p; }
  int main(){
      int a = 42;
      return *get_ptr(&a);
  }
  
