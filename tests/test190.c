// EXPECT: 5
// SECTION: ポインタ型返り値の関数

  int **get_pp(int **pp){ return pp; }
  int main(){
      int a = 5;
      int *p = &a;
      int **pp = get_pp(&p);
      return **pp;
  }
  
