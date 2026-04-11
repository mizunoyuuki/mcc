// EXPECT: 55
// SECTION: ブロック内 return / ブロック外 return

int main(){
    int a = 55;
    {
        int b = 0;
        b = a;
        return b;
    }
}

