// EXPECT: 5
// SECTION: ポインタ型変数

int main(){
    int a = 5;
    int *p = &a;
    int **pp = &p;
    return **pp;
}

