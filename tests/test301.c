// EXPECT: 42
// SECTION: ブロック内 return / ブロック外 return

int main(){
    {
        int a = 42;
        return a;
    }
    return 0;
}

