// EXPECT: 8
// SECTION: sizeof(ポインタ型)

int main(){
	int a = 8;
	int b = 12;
	int *c = &b;
	return sizeof(c+2);
}
