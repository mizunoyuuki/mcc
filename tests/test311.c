// EXPECT: 30
// SECTION: ブロック内 return / ブロック外 return

int main(){
    int result = 0;
    {
        int x = 10;
        result = x;
    }
    {
        int y = 20;
        result = result + y;
    }
    return result;
}

