// EXPECT: 5
// SECTION: グローバル変数とローカル変数の名前衝突

int x;
int main(){
    int x = 5;
    return x;
}

