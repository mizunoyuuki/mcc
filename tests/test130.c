// EXPECT: 20
// SECTION: 配列からポインタへの暗黙変換

int main(){
    int a[3];
    *a = 10;
    *(a + 1) = 20;
    *(a + 2) = 30;
    int *p = a + 1;
    return *p;
}

