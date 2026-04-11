// EXPECT: 1
// SECTION: ポインタ型返り値の関数

int a[3];
int b[3];
int main(){
	*a = 1;
	*(a + 1) = 2;
	*b = 10;
	*(b + 1) = 20;
	return *a;
}

