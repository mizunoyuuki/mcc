// EXPECT: 3
// SECTION: ブロック内 return / ブロック外 return

int main(){
    int i = 0;
    while (i < 10) {
        if (i == 3) {
            {
                int captured = i;
                return captured;
            }
        }
        i = i + 1;
    }
    return 99;
}

