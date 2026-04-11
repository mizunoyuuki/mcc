// EXPECT: 10
// SECTION: 配列からポインタへの暗黙変換

int main(){
    int a[5];
    for (int i = 0; i < 5; i = i + 1)
        *(a + i) = 2;
    int sum = 0;
    for (int j = 0; j < 5; j = j + 1)
        sum = sum + *(a + j);
    return sum;
}

