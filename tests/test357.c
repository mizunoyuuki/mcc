// EXPECT: ERROR
// SECTION: void*のデリファレンスはコンパイルエラー
int main(){
    int x;
    void *ptr;
    x = 42;
    ptr = &x;
    return *ptr;
}
