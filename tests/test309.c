// EXPECT: 4
// SECTION: ブロック内 return / ブロック外 return

int main(){
    if (1) {
        if (1) {
            if (1) {
                int a = 4;
                return a;
            }
        }
    }
    return 0;
}

