// EXPECT: 5
// SECTION: グローバル変数

int a;
int main(){
    a = 0;
    while (a < 5)
        a = a + 1;
    return a;
}

