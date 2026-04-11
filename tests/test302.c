// EXPECT: 7
// SECTION: ブロック内 return / ブロック外 return

int main(){
    if (1) {
        int a = 7;
        return a;
    }
    return 0;
}

