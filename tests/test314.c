// EXPECT: 1
// SECTION: ブロック内 return / ブロック外 return

int main(){
    int a = 1;
    if (1) {
        int a = 99;
    }
    return a;
}

