// EXPECT: 99
// SECTION: 配列からポインタへの暗黙変換

int set_first(int *p){
    *p = 99;
    return 0;
}
int main(){
    int a[3];
    *a = 0;
    set_first(a);
    return *a;
}

