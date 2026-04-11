// EXPECT: 4
// SECTION: 単項演算子（&, *）

int main(){
	int a = 10;
	int *p = &a;
	*p = 4;
	return a;
}

