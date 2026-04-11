// EXPECT: 1
// SECTION: 配列の定義

int main(){
    int a[5];
    if (sizeof(a) == 20) return 1;
    return 0;
}

