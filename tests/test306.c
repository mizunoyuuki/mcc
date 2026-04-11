// EXPECT: 5
// SECTION: ブロック内 return / ブロック外 return

int main(){
    int i = 0;
    while (i < 100) {
        if (i == 5) {
            return i;
        }
        i = i + 1;
    }
    return 0;
}

