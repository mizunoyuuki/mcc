// EXPECT: 55
// SECTION: 関数定義（複合テスト）

int sum_to_ten(){
    int s = 0;
    int i = 1;
    while (i <= 10){
        s = s + i;
        i = i + 1;
    }
    return s;
}

int main(){
    return sum_to_ten();
}

