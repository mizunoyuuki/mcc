// EXPECT: 6
// SECTION: ポインタ型返り値の関数

int a[3];
int main(){
	*a = 1;
	*(a + 1) = 2;
	*(a + 2) = 3;
	return *a + *(a + 1) + *(a + 2);
}

