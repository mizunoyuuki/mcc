// EXPECT: 11
// SECTION: グローバル変数とローカル変数の名前衝突

int a;
int b;
int main(){
    a = 1;
    b = 2;
    int a = 3;
    int b = 4;
    return a + b + a + 1;
}

