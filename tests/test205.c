// EXPECT: 5
// SECTION: ポインタ型返り値の関数

char a[10];
int main(){
	*a = 5;
	return *a;
}

