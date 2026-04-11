// EXPECT: 10
// SECTION: ポインタ型変数（初期化なし宣言）

int main(){
	int a = 10;
	int b = 8;
	int *p = &b + 1;
	return *p;
}

