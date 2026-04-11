// EXPECT: 88
// SECTION: ポインタ型変数

int main(){
    int a = 0;
    int *p = &a;
    int **pp = &p;
    **pp = 88;
    return a;
}

