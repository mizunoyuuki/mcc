// EXPECT: 42
// SECTION: 関数引数にポインタ型

  int deref(int *p){ return *p; }
  int main(){
      int a = 42;
      return deref(&a);
  }
  
