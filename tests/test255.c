// EXPECT: 10
// SECTION: 論理否定演算子 !

int main(){
    int x = 0;
    if (!x) return 10;
    return 20;
}

