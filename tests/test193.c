// EXPECT: 55
// SECTION: ポインタ型返り値の関数

  int g;
  int *get_gptr(){ return &g; }
  int main(){
      *get_gptr() = 55;
      return g;
  }
  
