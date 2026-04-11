// EXPECT: 7
// SECTION: ポインタ型返り値の関数

int a[3];
int set_vals(){
	*a = 3;
	*(a + 1) = 4;
	return 0;
}
int main(){
	set_vals();
	return *a + *(a + 1);
}

