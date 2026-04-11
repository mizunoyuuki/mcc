// EXPECT: 3
// SECTION: 単項演算子（&, *）

int main(){
    int a = 3;
    int *b = &a;
    int **c = &b;
    return **c;
}

