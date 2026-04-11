// EXPECT: 60
// SECTION: 配列からポインタへの暗黙変換

int main(){
    int a[3];
    *a = 10;
    *(a + 1) = 20;
    *(a + 2) = 30;
    return *a + *(a + 1) + *(a + 2);
}

