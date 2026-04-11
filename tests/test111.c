// EXPECT: 1
// SECTION: 配列の定義

int main(){
    int a[10];
    if (sizeof(a) == 40) return 1;
    return 0;
}

