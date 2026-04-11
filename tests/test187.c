// EXPECT: 7
// SECTION: ポインタ型返り値の関数

  char *get_ptr(char *p){ return p; }
  int main(){
      char a = 7;
      return *get_ptr(&a);
  }
  
