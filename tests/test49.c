// EXPECT: 30
// SECTION: 単項演算子（&, *）

int main(){
    int a = 10;
    int b = 20;
    return *(&a) + *(&b);
}

