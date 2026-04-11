// EXPECT: 60
// SECTION: グローバル変数（複合テスト）

int g;
int main(){
    int arr[3];
    *arr = 10;
    *(arr + 1) = 20;
    *(arr + 2) = 30;
    g = *arr + *(arr + 1) + *(arr + 2);
    return g;
}

