// EXPECT: 3
// SECTION: ポインタ型変数（初期化なし宣言）

int main(){
    int x;
    int *y;
    y = &x;
    *y = 3;
    return x;
}

