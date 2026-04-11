// EXPECT: 30
// SECTION: 配列からポインタへの暗黙変換

int main(){
    int a[2];
    int b[2];
    *a = 10;
    *(a + 1) = 20;
    *b = 100;
    *(b + 1) = 200;
    return *a + *(a + 1);
}

