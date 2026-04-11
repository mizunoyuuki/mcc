// EXPECT: 99
// SECTION: ポインタ型返り値の関数

int a[3];
int main(){
	*a = 0;
	int *p = a;
	*p = 99;
	return *a;
}

