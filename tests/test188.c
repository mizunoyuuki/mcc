// EXPECT: 10
// SECTION: ポインタ型返り値の関数

  int *identity(int *p){ return p; }
  int main(){
      int a = 10;
      int *p = identity(&a);
      return *p;
  }
  
