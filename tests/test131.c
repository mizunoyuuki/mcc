// EXPECT: 1
// SECTION: 配列からポインタへの暗黙変換

int main(){
    int a[2];
    *a = 10;
    *(a + 1) = 20;
    if (*a == 10) return 1;
    return 0;
}

