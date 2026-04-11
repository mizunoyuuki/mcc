// EXPECT: 25
// SECTION: ポインタ型グローバル変数

  char *gcp;
  int main(){
      char a = 25;
      gcp = &a;
      return *gcp;
  }
  
