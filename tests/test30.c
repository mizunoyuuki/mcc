// EXPECT: 40
// SECTION: for + if/else
int main(){
int a=0;
int b=0;
for (int i=0; i<4; i= i+1){
	a = a+10;
	b = b+11;
	if (a > b){
		a = a + 1;
	} else {
	        b = b + 1;
        }
}

a;
}
