// EXPECT: 20
// SECTION: ポインタ型返り値の関数

  int *second(int *p){ return p + 1; }
  int main(){
      int a[2];
      *a = 10;
      *(a + 1) = 20;
      return *second(a);
  }
  
