// EXPECT: 3
// SECTION: ブロック内 return / ブロック外 return

int main(){
    int a = 1;
    {
        int b = 2;
        {
            int c = 3;
        }
        return a + b;
    }
    return 0;
}

