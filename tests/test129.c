// EXPECT: 42
// SECTION: 配列からポインタへの暗黙変換

int main(){
    int a[3];
    *a = 42;
    int *p = a;
    int *q = a;
    return *q;
}

