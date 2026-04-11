// EXPECT: 10
// SECTION: ポインタ型返り値の関数

int a[5];
int main(){
	int i;
	for (i = 0; i < 5; i = i + 1)
		*(a + i) = 2;
	int sum = 0;
	for (i = 0; i < 5; i = i + 1)
		sum = sum + *(a + i);
	return sum;
}

