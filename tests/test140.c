// EXPECT: 10
// SECTION: グローバル変数

int a;
int main(){
    a = 0;
    for (int i = 0; i < 10; i = i + 1)
        a = a + 1;
    return a;
}

