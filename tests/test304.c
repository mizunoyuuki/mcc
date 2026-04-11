// EXPECT: 2
// SECTION: ブロック内 return / ブロック外 return

int main(){
    if (0) {
        return 1;
    } else {
        int a = 2;
        return a;
    }
    return 0;
}

