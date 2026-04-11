// EXPECT: 5
// SECTION: ポインタ型変数（初期化なし宣言）

int main(){
    int a = 5;
    int *p = &a;
    int **pp;
    pp = &p;
    return **pp;
}

