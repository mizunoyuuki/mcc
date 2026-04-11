// EXPECT: 50
// SECTION: char型ポインタ

  int main(){
      char a = 1;
      char *p = &a;
      *p = 50;
      return a;
  }
  
