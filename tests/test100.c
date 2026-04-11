// EXPECT: 55
// SECTION: int/char/ptr サイズの混在確認

  int main(){
      int a = 50;
      char b = 5;
      int *p = &a;
      return *p + b;
  }
  
