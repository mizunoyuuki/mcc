// EXPECT: 3
// SECTION: 配列からポインタへの暗黙変換

int main(){
    int a[2];
    *a = 1;
    *(a + 1) = 2;
    int *p;
    p = a;
    return *p + *(p + 1);
}

