// EXPECT: 30
// SECTION: グローバル変数（複合テスト）

int x;
int y;
int swap_via_ptr(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
    return 0;
}
int main(){
    x = 20;
    y = 10;
    swap_via_ptr(&x, &y);
    return x + y;
}

