// EXPECT: 0
// SECTION: void*ポインタ変数定義は有効
int main(){
    int x;
    void *ptr;
    x = 42;
    ptr = &x;
    return 0;
}
