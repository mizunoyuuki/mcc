// EXPECT: 10
// SECTION: ブロック内 return / ブロック外 return

int main(){
    int i = 0;
    int sum = 0;
    while (i < 5) {
        int tmp = i;
        sum = sum + tmp;
        i = i + 1;
    }
    return sum;
}

