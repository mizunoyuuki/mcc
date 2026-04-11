// EXPECT: 77
// SECTION: ポインタ型返り値の関数

  int g;
  int *get_gptr(){ return &g; }
  int main(){
      g = 77;
      return *get_gptr();
  }
  
