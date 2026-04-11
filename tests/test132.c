// EXPECT: 3
// SECTION: 配列からポインタへの暗黙変換

  int main(){
      int a[2];
      a[0] = 1;
      a[1] = 2;
      return a[0] + a[1];
  }
  
