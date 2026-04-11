// EXPECT: 11
// SECTION: if/else
int main(){
int a = 10;
int b = 0;
if (a == b)
	return b+3;
else if (a != b + 10)
	return a;
else
	return 11;
}
