// EXPECT: 6
// SECTION: ポインタ型返り値の関数

int sum(int *p){
	return *p + *(p + 1) + *(p + 2);
}
int a[3];
int main(){
	*a = 1;
	*(a + 1) = 2;
	*(a + 2) = 3;
	return sum(a);
}

