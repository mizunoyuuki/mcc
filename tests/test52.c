// EXPECT: 77
// SECTION: 単項演算子（&, *）

int f(int x){
    return *(&x);
}
int main(){
    return f(77);
}

