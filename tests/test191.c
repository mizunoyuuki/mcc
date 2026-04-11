// EXPECT: 30
// SECTION: ポインタ型返り値の関数

  int *get_ptr(int *p){ return p; }
  int main(){
      int a = 10;
      int b = 20;
      int *pa = get_ptr(&a);
      int *pb = get_ptr(&b);
      return *pa + *pb;
  }
  
