// EXPECT: 7
// SECTION: ポインタ型返り値の関数

int a[10];
int main(){
	*a = 1;
	*(a + 1) = 2;
	*(a + 2) = 3;
	*(a + 3) = 4;
	*(a + 4) = 5;
	*(a + 5) = 6;
	*(a + 6) = 7;
	return *(a + 6);
}

