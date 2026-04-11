// EXPECT: 5
// SECTION: 関数引数にポインタ型

  int read_next(int *p){ return *(p + 1); }
  int main(){
      int a = 5;
      int b = 7;
      return read_next(&b);
  }
  
