// EXPECT: 99
// SECTION: ポインタ型変数

int main(){
    int a = 1;
    int *p = &a;
    *p = 99;
    return a;
}

