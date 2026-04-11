// EXPECT: 111
// SECTION: 文字列リテラル

int main(){
    char *s = "hello";
    return *(s + 4);
}

