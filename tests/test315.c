// EXPECT: 8
// SECTION: ブロック内 return / ブロック外 return

int main(){
    int i = 0;
    int found = 0;
    while (i < 10) {
        if (i == 8) {
            int tmp = i;
            found = tmp;
        }
        i = i + 1;
    }
    return found;
}

