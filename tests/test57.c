// EXPECT: 7
// SECTION: ポインタ型変数

int main(){
    int a = 7;
    int *p = &a;
    int *q = p;
    return *q;
}

