// EXPECT: 5
// SECTION: 単項演算子（&, *）

int main(){
    int a = 5;
    int b = 7;
    int *c = &b;
    int *z = c + 1;
    return *z;
}

