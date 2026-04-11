// EXPECT: 55
// SECTION: 配列の定義

int main(){
    int arr[3];
    int a = 0;
    int *p = &a;
    *p = 55;
    return a;
}

