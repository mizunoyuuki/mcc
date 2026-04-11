// EXPECT: 15
// SECTION: 配列からポインタへの暗黙変換

int sum(int *p){
    return *p + *(p + 1) + *(p + 2);
}
int main(){
    int a[3];
    *a = 3;
    *(a + 1) = 5;
    *(a + 2) = 7;
    return sum(a);
}

