// EXPECT: 15
// SECTION: int/char/ptr サイズの混在確認

  int mixed(int a, char b){
      return a + b;
  }
  int main(){
      return mixed(10, 5);
  }
  
