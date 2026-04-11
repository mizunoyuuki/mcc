// EXPECT: 30
// SECTION: ポインタ型変数

int main(){
    int a = 10;
    int b = 20;
    int *pa = &a;
    int *pb = &b;
    return *pa + *pb;
}

