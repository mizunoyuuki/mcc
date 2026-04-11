// EXPECT: 1
// SECTION: 関数定義（複合テスト）

int check(int x, int y){
    if (1 == 1)
        return 1;
    else
        return 0;
}

int main(){
    return check(10, 20);
}

