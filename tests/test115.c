// EXPECT: 10
// SECTION: 配列の定義

int main(){
    int arr[5];
    int sum = 0;
    for (int i = 0; i < 5; i = i + 1)
        sum = sum + 2;
    return sum;
}

