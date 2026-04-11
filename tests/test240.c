// EXPECT: 1
// SECTION: 文字列リテラル

int main(){
    char *s = "hello";
    if (*s == 'h') return 1;
    return 0;
}

