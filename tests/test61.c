// EXPECT: 15
// SECTION: ポインタ型変数

int main(){
    int a = 10;
    int *p = &a;
    int b = *p + 5;
    return b;
}

