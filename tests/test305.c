// EXPECT: 5
// SECTION: ブロック内 return / ブロック外 return

int main(){
    int r = 0;
    if (1) {
        int a = 5;
        r = a;
    } else {
        int a = 10;
        r = a;
    }
    return r;
}

