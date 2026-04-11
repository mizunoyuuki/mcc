// EXPECT: 176
// SECTION: 配列からポインタへの暗黙変換

int main(){
    int x = 99;
    int a[3];
    *a = 10;
    *(a + 1) = 20;
    *(a + 2) = 30;
    int y = 77;
    return x + y;
}

