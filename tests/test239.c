// EXPECT: 1
// SECTION: 文字列リテラル

int main(){
    char *s = "hello";
    if (*s == 104) return 1;
    return 0;
}

