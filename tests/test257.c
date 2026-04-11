// EXPECT: 5
// SECTION: 論理否定演算子 !

int main(){
    int x = 0;
    int i = 0;
    while (!x){
        i = i + 1;
        if (i == 5) x = 1;
    }
    return i;
}

