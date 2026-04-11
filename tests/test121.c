// EXPECT: 1
// SECTION: 配列からポインタへの暗黙変換

int main(){
    int a[5];
    if (sizeof(a) == 20) return 1;
    return 0;
}

