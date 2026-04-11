// EXPECT: 3
// SECTION: 関数引数にポインタ型

int deref(int **pp){ return **pp; }
int main(){
	int a = 3;
	int *p = &a;
	return deref(&p);
}

