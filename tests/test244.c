// EXPECT: 0
// SECTION: 文字列リテラル

int main(){
    char *s = "hello";
    char *p = s;
    while (*p != 0)
        p = p + 1;
    return *p;
}

