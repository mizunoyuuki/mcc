// EXPECT: 10
// SECTION: char型変数

  int main(){
      char a = 0;
      for (int i = 0; i < 10; i = i + 1)
          a = a + 1;
      return a;
  }
  
