// EXPECT: 15
// SECTION: ポインタ型返り値の関数

  int *get_ptr(int *p){ return p; }
  int main(){
      int a[3];
      *a = 1;
      *(a + 1) = 2;
      *(a + 2) = 3;
      int sum = 0;
      for (int i = 0; i < 3; i = i + 1)
          sum = sum + *get_ptr(a + i);
      return sum + 9;
  }
  
