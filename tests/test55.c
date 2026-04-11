// EXPECT: 42
// SECTION: ポインタ型変数

int main(){
    int a = 42;
    int *p;
    p = &a;
    return *p;
}

