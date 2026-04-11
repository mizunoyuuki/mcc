// EXPECT: 5
// SECTION: 文字列リテラル

int strlen_impl(char *s){
    int n = 0;
    while (*(s + n) != 0)
        n = n + 1;
    return n;
}
int main(){
    return strlen_impl("hello");
}

