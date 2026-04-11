// EXPECT: 6
// SECTION: 配列からポインタへの暗黙変換

int main(){
    char a[3];
    *a = 1;
    *(a + 1) = 2;
    *(a + 2) = 3;
    return *a + *(a + 1) + *(a + 2);
}

