// EXPECT: 33
// SECTION: char型ポインタ

  int deref_char(char *p){ return *p; }
  int main(){
      char a = 33;
      return deref_char(&a);
  }
  
