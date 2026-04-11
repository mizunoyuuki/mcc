// EXPECT: 10
// SECTION: ブロック内 return / ブロック外 return

int main(){
    int sum = 0;
    if (1) {
        sum = sum + 10;
    }
    return sum;
}

