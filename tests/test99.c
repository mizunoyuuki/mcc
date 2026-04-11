// EXPECT: 1
// SECTION: int/char/ptr サイズの混在確認

  int main(){
      if (sizeof(char) == 1)
          if (sizeof(int) == 4)
              return 1;
      return 0;
  }
  
