// EXPECT: 30
// SECTION: 関数引数にポインタ型

  int add_deref(int *p, int b){ return *p + b; }
  int main(){
      int a = 10;
      return add_deref(&a, 20);
  }
  
